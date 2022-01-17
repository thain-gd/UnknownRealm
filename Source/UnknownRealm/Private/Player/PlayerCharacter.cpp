// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CraftingComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Core/MPGameInstance.h"
#include "Core/MPPlayerController.h"
#include "Equips/Equipment.h"
#include "Equips/RangeWeapon.h"
#include "Equips/Sword.h"
#include "Equips/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/CollectibleItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/InteractionWidget.h"

#define COLLISION_ENEMY ECC_GameTraceChannel1

// Sets default values
APlayerCharacter::APlayerCharacter()
	: AimingFOV(50.0f), AimingInterpSpeed(20.0f), CounterReduction(0.8f)
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

	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComp"));
	AddOwnedComponent(StaminaComp);

	CraftingComp = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComp"));
	CraftingComp->SetIsReplicated(true);
	AddOwnedComponent(CraftingComp);
}

void APlayerCharacter::ShowDamageDealt(const float InDealtDamage) const
{
	if (GetController()->IsLocalPlayerController())
	{
		GetGameInstance<UMPGameInstance>()->ShowDamage(InDealtDamage);
	}
	else
	{
		CL_ShowDamageDealt(InDealtDamage);
	}
}

void APlayerCharacter::CL_ShowDamageDealt_Implementation(const float InDealtDamage) const
{
	GetGameInstance<UMPGameInstance>()->ShowDamage(InDealtDamage);
}

void APlayerCharacter::EnableImmobility()
{
	SR_PutWeaponAway();
	GetCharacterMovement()->DisableMovement();
	Weapon->CL_DisableInput(Cast<APlayerController>(GetController()));
}

void APlayerCharacter::DisableImmobility() const
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Weapon->CL_EnableInput(Cast<APlayerController>(GetController()));
}

EWeaponType APlayerCharacter::GetEquippedWeaponType() const
{
	if (!IsValid(Weapon) || !Weapon->IsWeaponActive())
		return EWeaponType::None;
	
	return Weapon->GetWeaponType();
}

float APlayerCharacter::GetHealth() const
{
	return HealthComp->GetRemainingHealth();
}

float APlayerCharacter::GetHealthPercent() const
{
	return HealthComp->GetRemainingHealthPercent();
}

float APlayerCharacter::GetStaminaPercent() const
{
	return StaminaComp->GetCurrentStaminaPercent();
}

void APlayerCharacter::CL_SetRecoverableHealth_Implementation(float InRecoverableHealth)
{
	RecoverableHealth += InRecoverableHealth;
	HealthRecoveryAmount = RecoverableHealth * 0.1f;
	bCanStartRecoveryHealth = true;
}

float APlayerCharacter::GetRecoverableHealthPercent() const
{
	return RecoverableHealth / HealthComp->GetMaxHealth();
}

bool APlayerCharacter::IsDead() const
{
	return !HealthComp->IsAlive();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		HealthComp->OnHealthChanged.BindDynamic(this, &APlayerCharacter::OnHealthChanged);
	}

	if (IsLocallyControlled())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::ShowInteractingUI);
		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::HideInteractingUI);
		
		SR_EquipWeapon(this, WeaponID);
	}

	DefaultFOV = CameraComp->FieldOfView;
	DefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	AimingMovingSpeed = DefaultMoveSpeed * 0.45f;
	SprintSpeed = DefaultMoveSpeed * 1.5f;
	
	CraftingComp->Init(CameraComp);
}

void APlayerCharacter::OnHealthChanged()
{
	if (HealthComp->IsAlive())
	{
		// TODO: Play damaged animation + SFX
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged. Remaining: %f"), HealthComp->GetRemainingHealth());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Died"));
	}
}

void APlayerCharacter::SR_EquipWeapon_Implementation(AActor* InWeaponOwner, const FName& InWeaponID)
{
	if (Weapon)
	{
		GetWorld()->DestroyActor(Weapon);
		Weapon = nullptr;
	}
	
	FEquipmentInfo* WeaponInfo = GetGameInstance<UMPGameInstance>()->GetWeaponData()->FindRow<FEquipmentInfo>(InWeaponID, TEXT("APlayerCharacter::SetupWeapon"));
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponInfo->Class);
	if (!Weapon)
	{
		return;
	}
	
	Weapon->SetOwner(InWeaponOwner);
	Weapon->Init(WeaponInfo);
	Weapon->CL_SetupInputs();
}

void APlayerCharacter::ShowInteractingUI(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor,
	UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	ACollectibleItem* Item = Cast<ACollectibleItem>(InOtherActor);
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

void APlayerCharacter::HideInteractingUI(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor,
	UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex)
{
	ACollectibleItem* Item = Cast<ACollectibleItem>(InOtherActor);
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

void APlayerCharacter::Tick(float InDeltaSeconds)
{
	Super::Tick(InDeltaSeconds);

	if (!IsLocallyControlled() || !IsValid(Weapon))
		return;
	
	UpdateCameraFOV(InDeltaSeconds);

	if (!Weapon->IsAiming())
		return;

	const float InterpSpeed = 50.0f;
	const FRotator TargetRotation(0.0f, GetControlRotation().Yaw + 20.0f, 0.0f);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, InDeltaSeconds, InterpSpeed);
	SetPlayerRotation(NewRotation);
}

void APlayerCharacter::UpdateCameraFOV(float InDeltaSeconds)
{
	const bool bIsAiming = Weapon->IsAiming();
	
	const float TargetFOV = bIsAiming ? AimingFOV : DefaultFOV;
	const float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, InDeltaSeconds, AimingInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);

	const float TargetSocketOffsetY = bIsAiming ? 60.0f : 0.0f;
	const float NewSocketOffsetY = FMath::FInterpTo(SpringArmComp->SocketOffset.Y, TargetSocketOffsetY, InDeltaSeconds, AimingInterpSpeed);
	SpringArmComp->SocketOffset = FVector(SpringArmComp->TargetOffset.X, NewSocketOffsetY, SpringArmComp->TargetOffset.Z);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);

	InputComponent->BindAxis("MoveForward/Backward", this, &APlayerCharacter::MoveVertical);
	InputComponent->BindAxis("MoveRight/Left", this, &APlayerCharacter::MoveHorizontal);
	InputComponent->BindAxis("LookLeft/Right", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp/Down", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("Scroll", this, &APlayerCharacter::OnWheelAxisChanged);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);
	InputComponent->BindAction("SpaceActions", IE_Pressed, this, &APlayerCharacter::OnSpaceActionsPressed);
	
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	InputComponent->BindAction("Craft", IE_Pressed, this, &APlayerCharacter::ToggleCraftMenu);

	// Combat
	//InputComponent->BindAction("PutWeaponAway", IE_Pressed, this, &APlayerCharacter::SR_PutWeaponAway);
}

void APlayerCharacter::MoveVertical(float InAxisValue)
{
	if (bInteracting)
		return;
	
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, InAxisValue);
}

void APlayerCharacter::MoveHorizontal(float InAxisValue)
{
	if (bInteracting)
		return;
	
	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	AddMovementInput(Direction, InAxisValue);
}

void APlayerCharacter::StartSprinting()
{
	StaminaComp->DecreaseStaminaByPoint(60.0f);
	
	SR_PutWeaponAway();

	SR_SetMovementSpeed(SprintSpeed);
}

void APlayerCharacter::StopSprinting()
{
	SR_SetMovementSpeed(DefaultMoveSpeed);
}

void APlayerCharacter::OnSpaceActionsPressed()
{
	// Ignore if pressed when stand still
	if (GetCharacterMovement()->GetLastInputVector() == FVector::ZeroVector)
		return;
	
	// While attacking, cannot do dodge roll and possible to do side step
	if (Weapon->IsAttacking())
	{
		if (!bCanSideStep)
			return;
		
		// Detect direction b/w inputs and the character to
		// trigger side steps properly for different camera orientation
		const FVector LastInputVector = GetCharacterMovement()->GetLastInputVector();
		const float InputAndActorForwardDot = FVector::DotProduct(LastInputVector, GetActorForwardVector());
		const float InputAndActorRightDot = FVector::DotProduct(LastInputVector, GetActorRightVector());
		const bool bIsForwardOrBackwardInput = FMath::Abs(InputAndActorForwardDot) > FMath::Abs(InputAndActorRightDot);
		if (bIsForwardOrBackwardInput)
			return;
		
		if (StaminaComp->DecreaseStaminaByPercentage(Weapon->GetSideStepStaminaPercent()))
		{
			bCanSideStep = false; // prevent continuous sidesteps
			const bool bIsLeft = InputAndActorRightDot < 0;
			SR_DoSideStep(bIsLeft);
		}
	}
	else
	{
		if (StaminaComp->DecreaseStaminaByPercentage(DodgeStaminaPercent))
		{
			SR_DodgeRoll();
		}
	}
}

void APlayerCharacter::SR_DodgeRoll_Implementation()
{
	if (IsMontagePlaying(DodgeRollMontage))
	{
		return;
	}

	MC_PlayMontage(DodgeRollMontage);
}

void APlayerCharacter::SR_DoSideStep_Implementation(bool bIsLeft)
{
	MC_PlayMontage(bIsLeft ? Weapon->GetLeftSideStepMontage() : Weapon->GetRightSideStepMontage());
}

void APlayerCharacter::SR_PutWeaponAway_Implementation()
{
	if (!Weapon->IsWeaponActive())
	{
		return;
	}
	
	Weapon->SetIsWeaponActive(false);
}

void APlayerCharacter::SetMovementForAiming() const
{
	MC_SetMovementSpeed(AimingMovingSpeed);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APlayerCharacter::ResetMovement() const
{
	MC_SetMovementSpeed(DefaultMoveSpeed);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter::SetPlayerRotation(const FRotator& InNewRotation)
{
	if (!HasAuthority())
	{
		SetActorRotation(InNewRotation);
	}
	SR_SetPlayerRotation(InNewRotation);
}

void APlayerCharacter::SR_SetPlayerRotation_Implementation(const FRotator& InRotation)
{
	SetActorRotation(InRotation);
}

void APlayerCharacter::MC_SetPlayerRotation_Implementation(const FRotator& InRotation)
{
	SetActorRotation(InRotation);
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
				SR_FinishCollecting(CollectibleItem);
			}
		}
	}
}

void APlayerCharacter::ToggleCraftMenu()
{
	CraftingComp->ToggleCraftingWidget();
}

void APlayerCharacter::OnWheelAxisChanged(float InAxisValue)
{
	if (InAxisValue != 0 && CraftingComp->IsCrafting())
	{
		CraftingComp->SR_RotateCraftingObject(InAxisValue);
	}
}

void APlayerCharacter::ActivateInvincibility()
{
	if (HasAuthority())
	{
		HealthComp->SetInvincibility(true);
	}
}

void APlayerCharacter::DeactivateInvincibility()
{
	if (HasAuthority())
	{
		HealthComp->SetInvincibility(false);
	}
}

bool APlayerCharacter::GetInvincibility() const
{
	return HealthComp->GetInvincibility();
}

float APlayerCharacter::GetChargeAmount() const
{
	return Cast<ARangeWeapon>(Weapon)->GetChargeAmount();
}

bool APlayerCharacter::CheckCounterAttack()
{
	if (!bIsInCounterFrame)
		return false;

	ASword* Sword = Cast<ASword>(Weapon);
	check(Sword != nullptr);
	Sword->AllowNextCounterStep();
	return true;
}

void APlayerCharacter::StartRecoverHealth()
{
	if (bCanStartRecoveryHealth && IsLocallyControlled())
	{
		bCanStartRecoveryHealth = false;
		GetWorldTimerManager().SetTimer(HealthRecoveryTimerHandle, this, &APlayerCharacter::RecoverHealth, 1.0f, true, 0.0f);
	}
}

void APlayerCharacter::RecoverHealth()
{
	RecoverableHealth -= HealthRecoveryAmount;
	HealthComp->SR_IncreaseHealth(HealthRecoveryAmount);
	if (RecoverableHealth <= 0)
	{
		CL_StopRecoverHealth();
	}
}

void APlayerCharacter::CL_StopRecoverHealth_Implementation()
{
	RecoverableHealth = 0;
	GetWorldTimerManager().ClearTimer(HealthRecoveryTimerHandle);
}

void APlayerCharacter::SR_FinishCollecting_Implementation(ACollectibleItem* InCollectedItem)
{
	InCollectedItem->OnFinishedCollecting();
}

void APlayerCharacter::RecoverFullHealth() const
{
	HealthComp->SR_IncreaseHealth(HealthComp->GetMaxHealth());
}

void APlayerCharacter::ToggleInvincibility(bool bInIsInvincible) const
{
	HealthComp->SetInvincibility(bInIsInvincible);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, Weapon);
}
