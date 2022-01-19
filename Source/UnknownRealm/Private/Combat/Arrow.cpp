#include "Combat/Arrow.h"

#include "Combat/BaseDamageType.h"
#include "Core/MPGameState.h"
#include "Kismet/GameplayStatics.h"

void AArrow::OnFired(const FVector& InTargetLocation)
{
	Super::OnFired(InTargetLocation);
}

void AArrow::OnProjectileHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult)
{
	if (InOtherActor->ActorHasTag(FName("Player")) || InOtherActor->IsA(StaticClass()))
	{
		// Add back to inventory if the looter is the player who shot
		if (bHit && InOtherActor == GetOwner())
		{
			AMPGameState* GameState = GetWorld()->GetGameState<AMPGameState>();
			if (GameState)
			{
				GameState->GetInventory()->AddItem(ID);
			}

			Destroy();
		}

		return;
	}
	
	Super::OnProjectileHit(InOverlappedComponent, InOtherActor, InOtherComp, InOtherBodyIndex, bInFromSweep, InSweepResult);

	if (InOtherActor->ActorHasTag(FName("AI")))
	{
		MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
		SetLifeSpan(3.5f);

		OnProjectileHitEnemy.Execute(InOtherActor);
	}

	const FVector NewLocation = InSweepResult.ImpactPoint - GetActorForwardVector() * 60.0f;
	SetActorLocation(NewLocation);

	AttachToActor(InOtherActor, FAttachmentTransformRules::KeepWorldTransform);
}
