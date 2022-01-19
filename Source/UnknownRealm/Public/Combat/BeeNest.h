// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Projectile.h"
#include "BeeNest.generated.h"

class UBlindEffectComponent;
class APlayerCharacter;
class USphereComponent;

UCLASS()
class UNKNOWNREALM_API ABeeNest : public AProjectile
{
	GENERATED_BODY()

public:
	ABeeNest();
	
protected:
	void BeginPlay() override;

	void OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult) override;

private:
	UFUNCTION()
	void OnEnterBeeSwarm(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult);

	void ApplyPoisonEffect(APlayerCharacter& PlayerCharacter) const;
	

private:
	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* BeeSwarmArea;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBlindEffectComponent> BlindEffectClass;
	
	UPROPERTY(EditAnywhere)
	float BeeSwarmExistingTime;

	UPROPERTY(EditAnywhere)
	float BeePoisonDamage;

	UPROPERTY(EditAnywhere)
	float BeePoisonDuration;

	UPROPERTY(EditAnywhere)
	float BeePoisonTriggerRate;
};
