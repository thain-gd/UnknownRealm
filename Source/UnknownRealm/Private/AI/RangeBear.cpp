#include "AI/RangeBear.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

ARangeBear::ARangeBear()
	: BeeNestSocketName(TEXT("BeeNestSocket"))
{
}

bool ARangeBear::CanThrowBeeNest() const
{
	return IsPlayerInRangeBounds(CurrentTargetPlayer, ThrowAttackUpperBound, ThrowAttackLowerBound);
}

void ARangeBear::SpawnBeeNest()
{
	BeeNest = GetWorld()->SpawnActor<AActor>(BeeNestClass);
	BeeNest->SetOwner(this);
	BeeNest->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BeeNestSocketName);
}

void ARangeBear::ThrowBeeNest()
{
	if (CurrentTargetPlayer)
	{
		BeeNest->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UProjectileMovementComponent* ProjectileMovementComp = Cast<UProjectileMovementComponent>(BeeNest->GetComponentByClass(UProjectileMovementComponent::StaticClass()));
		ProjectileMovementComp->ProjectileGravityScale = 1.0f;
		const FVector FlyingDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), CurrentTargetPlayer->GetActorLocation());
		ProjectileMovementComp->Velocity = FlyingDirection * 500.0f + FVector::UpVector * 400.0f;
	}
	else
	{
		GetWorld()->DestroyActor(BeeNest);
	}
	
	BeeNest = nullptr;
}
