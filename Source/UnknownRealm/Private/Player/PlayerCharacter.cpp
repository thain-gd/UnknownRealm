// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "AIs/AIChar.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::SetAttackableEnemy);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::ResetAttackableEnemy);
}

void APlayerCharacter::SetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("AI")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Add Enemy"));
		AttackableEnemies.Add(OtherActor);
	}
}

void APlayerCharacter::ResetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("AI")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Remove Enemy"));
		AttackableEnemies.Remove(OtherActor);
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
}

void APlayerCharacter::MoveVertical(float AxisValue)
{
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void APlayerCharacter::MoveHorizontal(float AxisValue)
{
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

	for (auto AttackableEnemy : AttackableEnemies)
	{
		float Damaged = UGameplayStatics::ApplyDamage(AttackableEnemy, 35, nullptr, this, UDamageType::StaticClass());
	}
}
