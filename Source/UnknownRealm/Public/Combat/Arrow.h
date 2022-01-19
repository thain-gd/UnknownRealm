// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Projectile.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AArrow : public AProjectile
{
	GENERATED_BODY()

public:
	void OnFired(const FVector& InTargetLocation) override;
	
	const FName& GetID() const { return ID; }

protected:
	void OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult) override;


private:
	UPROPERTY(EditDefaultsOnly)
	FName ID;
};
