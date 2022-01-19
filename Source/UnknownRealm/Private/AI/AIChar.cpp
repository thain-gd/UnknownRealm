// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIChar.h"

#include "GameplayTagContainer.h"
#include "Combat/BaseDamageType.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Core/MPGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AAIChar::AAIChar()
	: bFinishedAttacking(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	AddOwnedComponent(HealthComp);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// An identified tag for AI chars (for collision detections, etc.)
	Tags.Add(FName("AI"));
}

bool AAIChar::IsPlayerInRangeBounds(AActor* InPlayer, const float InUpperBound, const float InLowerBound) const
{
	const float DistanceSquared = UKismetMathLibrary::Vector_DistanceSquared(GetActorLocation(), InPlayer->GetActorLocation());
	return DistanceSquared >= InLowerBound * InLowerBound && DistanceSquared <= InUpperBound * InUpperBound;
}

// Called when the game starts or when spawned
void AAIChar::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AAIChar::OnGotHit);
	HealthComp->OnHealthChanged.BindDynamic(this, &AAIChar::OnHealthChanged);
}

void AAIChar::ApplySimpleDamage(AActor* InDamagedActor, const float InDamage)
{
	UGameplayStatics::ApplyDamage(InDamagedActor, InDamage, nullptr, this, UBaseDamageType::StaticClass());
}

bool AAIChar::CanDoBaseAttack() const
{
	return IsPlayerInRangeBounds(CurrentTargetPlayer, BaseAttackRange);
}

APlayerCharacter* AAIChar::GetPlayerInRange(const float InRange)
{
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = StartLocation + GetActorForwardVector() * InRange;
	FHitResult OutHitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), StartLocation, EndLocation, GetCapsuleComponent()->GetScaledCapsuleRadius(), FName("PlayerDefault"), false, IgnoredActors, EDrawDebugTrace::None, OutHitResult, true);
	return Cast<APlayerCharacter>(OutHitResult.Actor);
}

void AAIChar::OnGotHit(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser)
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(InDamageCauser))
	{
		if (CurrentTargetPlayer != PlayerChar)
		{
			TargetablePlayers.Add(PlayerChar);
		}
	}
}

void AAIChar::OnHealthChanged()
{
	if (HealthComp->IsAlive())
	{
		// TODO: Play damaged animation + SFX
	}
	else
	{
		Die();
	}
}

void AAIChar::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *GetName());
	// TODO: Play death animation
	SetLifeSpan(2.0f);

	AMPGameMode* GameMode = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->OnEnemyDied();
	}
}

// Called every frame
void AAIChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APlayerCharacter* AAIChar::PickNextTargetPlayer()
{
	if (TargetablePlayers.Num() == 0)
	{
		return nullptr;
	}
	
	APlayerCharacter* NextTargetPlayer = *TargetablePlayers.begin();
	TargetablePlayers.Remove(NextTargetPlayer);
	CurrentTargetPlayer = NextTargetPlayer;
	return CurrentTargetPlayer;
}

