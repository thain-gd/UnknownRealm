// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BeeNest.h"

#include "Combat/BaseDamageType.h"
#include "Kismet/GameplayStatics.h"

void ABeeNest::OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	Super::OnProjectileHit(InOverlappedComponent, InOtherActor, InOtherComp, InOtherBodyIndex, bInFromSweep, InSweepResult);

	if (InOtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyDamage(InOtherActor, Damage, nullptr, GetOwner(), UBaseDamageType::StaticClass());
	}

	// TODO: Create AoE effect that players will be blinded and poisoned if enter
	Destroy();
}
