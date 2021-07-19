// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
UCLASS()
class UNKNOWNREALM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void OnFired(const FVector& TargetLocation, float InTotalDamage);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnEnemyHit(AActor* Enemy);
	
	void AddProjectileMovementComponent(const FVector& TargetLocation);


private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	float FlyingSpeed;

	bool bHit;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	float TotalDamage;
};
