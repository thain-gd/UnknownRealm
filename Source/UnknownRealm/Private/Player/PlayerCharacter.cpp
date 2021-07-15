// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "AIs/AIChar.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CraftingComponent.h"
#include "Components/HealthComponent.h"
#include "Core/MPGameInstance.h"
#include "Core/MPPlayerController.h"
#include "Equips/Equipment.h"
#include "Equips/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/CollectibleItem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/InteractionWidget.h"

const FName APlayerCharacter::InactiveWeaponSocketName = FName("InactiveWeaponSocket");

// Sets default values
APlayerCharacter::APlayerCharacter()
	: AimingFOV(50.0f), AimingInterpSpeed(20.0f)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 580.0f, 0.0f); // ...at this rotation rate
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	// Create a follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TP_CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackBox->SetupAttachment(RootComponent);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	AddOwnedComponent(HealthComp);

	CraftingComp = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComp"));
	CraftingComp->SetIsReplicated(true);
	AddOwnedComponent(CraftingComp);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupWeapon();

		AttackBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::SetAttackableEnemy);
		AttackBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::ResetAttackableEnemy);
	}

	if (IsValid(Weapon) && InputComponent && IsLocallyControlled())
	{
		// Binding actions by weapon type
		if (Weapon->GetWeaponType() == EWeaponType::Bow)
		{
			InputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::ServerOnAimingPressed);
			InputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::ServerOnAimingReleased);
			InputComponent->BindAction("Charge", IE_Pressed, this, &APlayerCharacter::ServerOnChargingStart);
			InputComponent->BindAction("Charge", IE_Released, this, &APlayerCharacter::ServerOnChargingEnd);
		}
		else
		{
			InputComponent->BindAction("NormalAttack", IE_Pressed, this, &APlayerCharacter::ServerDoNormalAttack);
		}
	}

	DefaultFOV = CameraComp->FieldOfView;
	DefaultMovingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	AimingMovingSpeed = DefaultMovingSpeed * 0.45f;
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::ShowInteractingUI);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::HideInteractingUI);

	CraftingComp->Init(CameraComp);
}

void APlayerCharacter::SetupWeapon()
{
	FEquipmentInfo* WeaponInfo = GetGameInstance<UMPGameInstance>()->GetWeaponData()->FindRow<FEquipmentInfo>(WeaponID, TEXT("APlayerCharacter::SetupWeapon"));
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponInfo->Class);
	Weapon->Init(WeaponInfo);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, InactiveWeaponSocketName);
	Weapon->SetOwner(this);
}

void APlayerCharacter::SetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("AI")))
	{
		AttackableEnemies.Add(OtherActor);
	}
}

void APlayerCharacter::ResetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("AI")))
	{
		AttackableEnemies.Remove(OtherActor);
	}
}

void APlayerCharacter::ShowInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsLocallyControlled())
		return;
	
	ACollectibleItem* Item = Cast<ACollectibleItem>(OtherActor);
	if (Item)
	{
		bInteractable = true;
		CollectibleItem = Item;

		if (!InteractionWidget)
		{
			InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		}

		InteractionWidget->SetInteractText(CollectibleItem->GetInteractString());
		if (!InteractionWidget->IsInViewport())
		{
			InteractionWidget->AddToViewport();
		}
	}
}

void APlayerCharacter::HideInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsLocallyControlled())
		return;
	
	ACollectibleItem* Item = Cast<ACollectibleItem>(OtherActor);
	if (Item && Item == CollectibleItem)
	{
		bInteractable = bInteracting = false;
		CollectibleItem = nullptr;

		if (InteractionWidget)
		{
			InteractionWidget->RemoveFromViewport();
		}
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCameraFOV(DeltaSeconds);
}

void APlayerCharacter::UpdateCameraFOV(float DeltaTime)
{
	const float TargetFOV = bIsAiming ? AimingFOV : DefaultFOV;
	const float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, AimingInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);

	const float SpringArmTargetY = bIsAiming ? 60.0f : 0.0f;
	const float SpringArmNewY = FMath::FInterpTo(SpringArmComp->GetRelativeLocation().Y, SpringArmTargetY, DeltaTime, AimingInterpSpeed);
	SpringArmComp->SetRelativeLocation(FVector(SpringArmComp->GetRelativeLocation().X, SpringArmNewY, SpringArmComp->GetRelativeLocation().Z));
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward/Backward", this, &APlayerCharacter::MoveVertical);
	InputComponent->BindAxis("MoveRight/Left", this, &APlayerCharacter::MoveHorizontal);
	InputComponent->BindAxis("LookLeft/Right", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp/Down", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("Scroll", this, &APlayerCharacter::OnWheelAxisChanged);

	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	InputComponent->BindAction("Craft", IE_Pressed, this, &APlayerCharacter::ToggleCraftMenu);

	// Combat
	InputComponent->BindAction("PutWeaponAway", IE_Pressed, this, &APlayerCharacter::ServerPutWeaponAway);
}

void APlayerCharacter::MoveVertical(float AxisValue)
{
	if (bInteracting)
		return;
	
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void APlayerCharacter::MoveHorizontal(float AxisValue)
{
	if (bInteracting)
		return;
	
	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	AddMovementInput(Direction, AxisValue);
}



void APlayerCharacter::ServerDoNormalAttack_Implementation()
{
	if (!bUsingWeapon)
	{
		ServerGetWeapon();
		return;
	}

	if (AttackableEnemies.Num() == 0)
		return;

	for (auto AttackableEnemy : AttackableEnemies)
	{
		UGameplayStatics::ApplyDamage(AttackableEnemy, 35, nullptr, this, UDamageType::StaticClass());
	}
}

void APlayerCharacter::ServerGetWeapon_Implementation()
{
	// TODO: Switch to using weapon animation
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon->GetAttachPoint());

	bUsingWeapon = true;
}

void APlayerCharacter::ServerDoHeavyAttack_Implementation()
{
	if (!bUsingWeapon)
		return;

	UE_LOG(LogTemp, Warning, TEXT("DoHeavyAttack"));
}

void APlayerCharacter::ServerPutWeaponAway_Implementation()
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, InactiveWeaponSocketName);
	bUsingWeapon = false;
}



void APlayerCharacter::ServerOnAimingPressed_Implementation()
{
	if (!bUsingWeapon)
		return;

	bIsAiming = true;
	OnAimingStart();
}

void APlayerCharacter::ServerOnAimingReleased_Implementation()
{
	if (!bUsingWeapon)
		return;

	bIsAiming = false;
	OnAimingEnd();
}

void APlayerCharacter::OnRepAimingStatusChanged()
{
	if (bIsAiming)
	{
		OnAimingStart();
	}
	else
	{
		OnAimingEnd();
	}
}

void APlayerCharacter::OnAimingStart()
{
	GetCharacterMovement()->MaxWalkSpeed = AimingMovingSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void APlayerCharacter::OnAimingEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovingSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::ServerOnChargingStart_Implementation()
{
	if (!bUsingWeapon)
	{
		ServerGetWeapon();
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Start Charging"));
}

void APlayerCharacter::ServerOnChargingEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("End Charging"));
}

void APlayerCharacter::Interact()
{
	if (CraftingComp->IsCrafting())
	{
		CraftingComp->VerifyPlacement();
		return;
	}
	
	if (bInteractable && CollectibleItem)
	{
		AMPPlayerController* PlayerController = Cast<AMPPlayerController>(Controller);
		if (!PlayerController)
			return;
		
		if (bInteracting)
		{
			InteractionWidget->CancelCollecting();
			bInteracting = false;
			return;
		}

		if (CollectibleItem->GetCollectionTime() > 0)
		{
			bInteracting = true;
			InteractionWidget->OnStartCollecting(CollectibleItem);
		}
		else
		{
			if (HasAuthority())
			{
				CollectibleItem->OnFinishedCollecting();
			}
			else
			{
				ServerFinishCollecting(CollectibleItem);
			}
		}
	}
}

void APlayerCharacter::ToggleCraftMenu()
{
	CraftingComp->ToggleCraftingWidget();
}

void APlayerCharacter::OnWheelAxisChanged(float AxisValue)
{
	if (AxisValue != 0 && CraftingComp->IsCrafting())
	{
		CraftingComp->ServerRotateCraftingObject(AxisValue);
	}
}

void APlayerCharacter::ServerFinishCollecting_Implementation(ACollectibleItem* CollectedItem)
{
	CollectedItem->OnFinishedCollecting();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, Weapon);
	DOREPLIFETIME(APlayerCharacter, bIsAiming);
}
