// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Projectile.h"

#include "DrawDebugHelpers.h"
#include "Core/MPGameInstance.h"
#include "Core/MPGameState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
	: FlyingSpeed(4000)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = MeshComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;

	bReplicates = true;
	SetReplicatingMovement(true);
}

void AProjectile::OnFired(const FVector& InTargetLocation)
{
	MeshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	const FVector FlyingDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), InTargetLocation);
	ProjectileMovementComp->Velocity = FlyingDirection * FlyingSpeed + FVector::UpVector * VerticalSpeed;
	ProjectileMovementComp->ProjectileGravityScale = ProjectileGravityScale;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileHit);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor,
	UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	// Prevent multiple overlapping occur and ignore owner
	if (bHit || InOtherActor == GetOwner())
	{
		return;
	}

	bHit = true;
	ProjectileMovementComp->DestroyComponent();
	
	// TODO: Play hit SFX
}
