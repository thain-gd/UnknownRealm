#include "AI/RangeBear.h"

#include "Combat/BeeNest.h"
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
	BeeNest = GetWorld()->SpawnActor<ABeeNest>(BeeNestClass);
	BeeNest->SetOwner(this);
	BeeNest->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BeeNestSocketName);
}

void ARangeBear::ThrowBeeNest()
{
	if (CurrentTargetPlayer)
	{
		BeeNest->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		BeeNest->OnFired(CurrentTargetPlayer->GetActorLocation());
	}
	else
	{
		GetWorld()->DestroyActor(BeeNest);
	}
	
	BeeNest = nullptr;
}
