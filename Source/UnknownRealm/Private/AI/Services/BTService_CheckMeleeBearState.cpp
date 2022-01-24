#include "AI/Services/BTService_CheckMeleeBearState.h"

#include "AIController.h"
#include "AI/MeleeBear.h"
#include "Player/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckMeleeBearState::UBTService_CheckMeleeBearState()
{
	NodeName = TEXT("Check Melee Bear Combat State");
}

void UBTService_CheckMeleeBearState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMeleeBear* MeleeBear = OwnerComp.GetAIOwner()->GetPawn<AMeleeBear>();
	if (!MeleeBear)
	{
		return;
	}

	// Update target player key again before checking states
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject(TargetPlayerKey.SelectedKeyName, MeleeBear->GetCurrentTargetPlayer());
	
	const EMeleeBearState State = static_cast<EMeleeBearState>(BlackboardComponent->GetValueAsEnum(StateKey.SelectedKeyName));
	switch (State)
	{
	case EMeleeBearState::Approach:
		if (AActor* PlayerCharacter = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetPlayerKey.SelectedKeyName)))
		{
			if (MeleeBear->CanRun())
			{
				MeleeBear->StartRunning();
				SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Run));
			}
			else if (MeleeBear->CanDoBaseAttack())
			{
				MeleeBear->StartAttacking();
				SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Slash));
			}
		}
		break;

	case EMeleeBearState::Run:
		if (MeleeBear->CanDoPounceAttack())
		{
			MeleeBear->StartAttacking();
			SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Pounce));
		}
		break;

	case EMeleeBearState::Pounce:
		if (MeleeBear->IsFinishedAttacking())
		{
			if (MeleeBear->IsChomping())
			{
				MeleeBear->StartAttacking();
				SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Chomp));
			}
			else
			{
				SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Approach));
			}
		}
		break;
		
	case EMeleeBearState::Slash:
		if (MeleeBear->IsFinishedAttacking())
		{
			if (MeleeBear->CanDoBaseAttack())
			{
				MeleeBear->StartAttacking();
			}
			else
			{
				SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Approach));
			}
		}
		break;

	case EMeleeBearState::Chomp:
		// Only return to approach state if not chomping anymore
		if (!MeleeBear->IsChomping())
		{
			SetState(*BlackboardComponent, static_cast<uint8>(EMeleeBearState::Approach));
		}
		else
		{
			MeleeBear->StartAttacking();
		}
		break;
	}
}
