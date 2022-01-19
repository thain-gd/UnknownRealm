// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnProjectileHitEnemy, AActor*, Enemy);

class UProjectileMovementComponent;
UCLASS(Abstract)
class UNKNOWNREALM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void OnFired(const FVector& InTargetLocation);

	float GetDamage() const { return Damage; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult);


public:
	FOnProjectileHitEnemy OnProjectileHitEnemy;
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float Damage;

	bool bHit;

private:
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere)
	float ProjectileGravityScale;

	UPROPERTY(EditAnywhere)
	float FlyingSpeed;

	UPROPERTY(EditAnywhere)
	float VerticalSpeed;
};
