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
	
	const EMeleeBearState State = static_cast<EMeleeBearState>(BlackboardComponent->GetValueAsEnum(CombatStateKey.SelectedKeyName));
	switch (State)
	{
	case EMeleeBearState::Approach:
		if (AActor* PlayerCharacter = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetPlayerKey.SelectedKeyName)))
		{
			if (MeleeBear->CanRun())
			{
				MeleeBear->StartRunning();
				ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Run);
			}
			else if (MeleeBear->CanDoBaseAttack())
			{
				MeleeBear->StartAttacking();
				ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Slash);
			}
		}
		break;

	case EMeleeBearState::Run:
		if (MeleeBear->CanDoPounceAttack())
		{
			MeleeBear->StartAttacking();
			ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Pounce);
		}
		break;
		
	case EMeleeBearState::Pounce:
		if (MeleeBear->IsFinishedAttacking())
		{
			if (MeleeBear->IsChomping())
			{
				MeleeBear->StartAttacking();
				ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Chomp);
			}
			else
			{
				ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Approach);
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
				ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Approach);
			}
		}
		break;
		
	case EMeleeBearState::Chomp:
		// Only return to approach state if not chomping anymore
		if (!MeleeBear->IsChomping())
		{
			ChangeMeleeBearState(*BlackboardComponent, EMeleeBearState::Approach);
		}
		else
		{
			MeleeBear->StartAttacking();
		}
		break;
	}
}

void UBTService_CheckMeleeBearState::ChangeMeleeBearState(UBlackboardComponent& InBlackboardComponent, EMeleeBearState InNewState) const
{
	InBlackboardComponent.SetValueAsEnum(CombatStateKey.SelectedKeyName, static_cast<uint8>(InNewState));
}
