// Fill out your copyright notice in the Description page of Project Settings.


#include "AIs/AIChar.h"

#include "GameplayTagContainer.h"
#include "Components/HealthComponent.h"

// Sets default values
AAIChar::AAIChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	HealthComp->OnHealthChanged.BindDynamic(this, &AAIChar::OnHealthChanged);
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
}

// Called every frame
void AAIChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

