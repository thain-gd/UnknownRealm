// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnProjectileHitEnemy, AActor*, Enemy);

class UProjectileMovementComponent;
UCLASS()
class UNKNOWNREALM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void OnFired(const FVector& TargetLocation, float InTotalDamage);

	const FName& GetID() const { return ID; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void AddProjectileMovementComponent(const FVector& TargetLocation);


public:
	FOnProjectileHitEnemy OnProjectileHitEnemy;
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	FName ID;

	UPROPERTY(EditAnywhere)
	float FlyingSpeed;

	bool bHit;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	float TotalDamage;
};
