// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BeeNest.h"

#include "Combat/BaseDamageType.h"
#include "Components/BlindEffectComponent.h"
#include "Components/OvertimeDamageComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

ABeeNest::ABeeNest()
{
	BeeSwarmArea = CreateDefaultSubobject<USphereComponent>(TEXT("SwarmAreaCollision"));
	BeeSwarmArea->SetCollisionProfileName(TEXT("NoCollision"));
	BeeSwarmArea->SetSphereRadius(150.0f);
	BeeSwarmArea->SetupAttachment(RootComponent);
}

void ABeeNest::BeginPlay()
{
	Super::BeginPlay();
	BeeSwarmArea->OnComponentBeginOverlap.AddDynamic(this, &ABeeNest::OnEnterBeeSwarm);
}

void ABeeNest::OnEnterBeeSwarm(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InOtherActor))
	{
		ApplyPoisonEffect(*PlayerCharacter);
		PlayerCharacter->CL_ApplyBlindEffect(BlindEffectClass);
	}
}

void ABeeNest::ApplyPoisonEffect(APlayerCharacter& PlayerCharacter) const
{
	UOvertimeDamageComponent* OvertimeDamageComp = PlayerCharacter.FindComponentByClass<UOvertimeDamageComponent>();
	if (!OvertimeDamageComp)
	{
		OvertimeDamageComp = NewObject<UOvertimeDamageComponent>(&PlayerCharacter, UOvertimeDamageComponent::StaticClass());
		check(OvertimeDamageComp != nullptr);
		OvertimeDamageComp->RegisterComponent();
		PlayerCharacter.AddOwnedComponent(OvertimeDamageComp);
	}

	OvertimeDamageComp->Init(GetOwner(), BeePoisonDamage, BeePoisonDuration, BeePoisonTriggerRate);
}

void ABeeNest::OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	Super::OnProjectileHit(InOverlappedComponent, InOtherActor, InOtherComp, InOtherBodyIndex, bInFromSweep, InSweepResult);
	
	if (InOtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyDamage(InOtherActor, Damage, nullptr, GetOwner(), UBaseDamageType::StaticClass());
	}

	// TODO: Separate this to spawn a separate bee swarm actor to prevent other nests overlap with the swarm
	MeshComp->SetVisibility(false);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	BeeSwarmArea->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetLifeSpan(BeeSwarmExistingTime);
}
