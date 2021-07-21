// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Projectile.h"

#include "DrawDebugHelpers.h"
#include "Core/MPGameInstance.h"
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

	bReplicates = true;
	SetReplicatingMovement(true);
}

void AProjectile::OnFired(const FVector& TargetLocation, float InTotalDamage)
{
	MeshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TotalDamage = InTotalDamage;

	AddProjectileMovementComponent(TargetLocation);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileHit);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHit || OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->IsA(StaticClass()))
		return;

	// TODO: Play hit SFX
	
	if (!HasAuthority())
		return;

	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
	
	bHit = true;
	ProjectileMovement->DestroyComponent();

	if (OtherActor->ActorHasTag(FName("AI")))
	{
		OnEnemyHit(OtherActor);
	}
	
	const FVector NewLocation = SweepResult.ImpactPoint - GetActorForwardVector() * 60.0f;
	SetActorLocation(NewLocation);
	
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
}

void AProjectile::OnEnemyHit(AActor* Enemy)
{
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	UGameplayStatics::ApplyDamage(Enemy, TotalDamage, nullptr, this, UDamageType::StaticClass());
	SetLifeSpan(3.5f);

	bool bIsCrit = false;
	if (GetOwner<APawn>()->GetController()->IsLocalPlayerController())
	{
		GetGameInstance<UMPGameInstance>()->ShowDamage(TotalDamage, bIsCrit);
	}
	else
	{
		ClientShowEnemyDamageTaken(TotalDamage, bIsCrit);
	}
}

void AProjectile::ClientShowEnemyDamageTaken_Implementation(int32 Damage, bool bIsCrit)
{
	GetGameInstance<UMPGameInstance>()->ShowDamage(Damage, bIsCrit);
}

void AProjectile::AddProjectileMovementComponent(const FVector& TargetLocation)
{
	ProjectileMovement = NewObject<UProjectileMovementComponent>(this, UProjectileMovementComponent::StaticClass());
	if (!ProjectileMovement)
		return;

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation));
	const FVector FlyingDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation);

	ProjectileMovement->RegisterComponent();
	ProjectileMovement->ProjectileGravityScale = 0.15f;
	ProjectileMovement->Velocity = FlyingDirection * FlyingSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	AddOwnedComponent(ProjectileMovement);
}


