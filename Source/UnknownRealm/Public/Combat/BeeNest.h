// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Projectile.h"
#include "BeeNest.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API ABeeNest : public AProjectile
{
	GENERATED_BODY()

protected:
	void OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult) override;
};
