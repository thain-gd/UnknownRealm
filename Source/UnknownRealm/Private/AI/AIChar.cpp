// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIChar.h"

#include "GameplayTagContainer.h"
#include "Components/HealthComponent.h"
#include "Core/MPGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AAIChar::AAIChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	AddOwnedComponent(HealthComp);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// An identified tag for AI chars (for collision detections, etc.)
	Tags.Add(FName("AI"));
}

// Called when the game starts or when spawned
void AAIChar::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AAIChar::OnGotHit);
	HealthComp->OnHealthChanged.BindDynamic(this, &AAIChar::OnHealthChanged);
}

void AAIChar::OnGotHit(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser)
{
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(InDamageCauser))
	{
		TargetablePlayers.Add(PlayerChar);
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

APlayerCharacter* AAIChar::RemoveFirstTargetPlayer()
{
	if (TargetablePlayers.Num() == 0)
	{
		return nullptr;
	}
	
	APlayerCharacter* FirstTargetPlayer = *TargetablePlayers.begin();
	TargetablePlayers.Remove(FirstTargetPlayer);
	return FirstTargetPlayer;
}

