// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "AIs/AIChar.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CraftingComponent.h"
#include "Components/HealthComponent.h"
#include "Core/MPGameState.h"
#include "Core/MPPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/CollectibleItem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractionWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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

	CraftingComp->Init(CameraComp);

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::SetAttackableEnemy);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::ResetAttackableEnemy);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::ShowInteractingUI);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::HideInteractingUI);
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

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward/Backward", this, &APlayerCharacter::MoveVertical);
	InputComponent->BindAxis("MoveRight/Left", this, &APlayerCharacter::MoveHorizontal);
	InputComponent->BindAxis("LookLeft/Right", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp/Down", this, &APawn::AddControllerPitchInput);

	InputComponent->BindAction("NormalAttack", IE_Pressed, this, &APlayerCharacter::Attack);
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	InputComponent->BindAction("Craft", IE_Pressed, this, &APlayerCharacter::ToggleCraftMenu);
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

void APlayerCharacter::Attack()
{
	if (AttackableEnemies.Num() == 0)
		return;

	ServerAttack();
}

void APlayerCharacter::ServerAttack_Implementation()
{
	for (auto AttackableEnemy : AttackableEnemies)
	{
		UGameplayStatics::ApplyDamage(AttackableEnemy, 35, nullptr, this, UDamageType::StaticClass());
	}
}

void APlayerCharacter::Interact()
{
	if (CraftingComp->IsCrafting())
	{
		CraftingComp->ServerVerifyPlacement();
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

void APlayerCharacter::ServerFinishCollecting_Implementation(ACollectibleItem* CollectedItem)
{
	CollectedItem->OnFinishedCollecting();
}
