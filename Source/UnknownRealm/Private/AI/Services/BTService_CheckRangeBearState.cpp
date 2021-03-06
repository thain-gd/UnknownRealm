// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_CheckRangeBearState.h"

#include "AIController.h"
#include "AI/RangeBear.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/PlayerCharacter.h"

UBTService_CheckRangeBearState::UBTService_CheckRangeBearState()
{
	NodeName = TEXT("Check Range Bear Combat State");
}

void UBTService_CheckRangeBearState::TickNode(UBehaviorTreeComponent& InOwnerComp, uint8* InNodeMemory,	float InDeltaSeconds)
{
	Super::TickNode(InOwnerComp, InNodeMemory, InDeltaSeconds);

	ARangeBear* RangeBear = InOwnerComp.GetAIOwner()->GetPawn<ARangeBear>();
	if (!RangeBear)
	{
		return;
	}

	// Update target player key again before checking states
	UBlackboardComponent* BlackboardComponent = InOwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject(TargetPlayerKey.SelectedKeyName, RangeBear->GetCurrentTargetPlayer());

	const ERangeBearState State = static_cast<ERangeBearState>(BlackboardComponent->GetValueAsEnum(CombatStateKey.SelectedKeyName));
	switch (State)
	{
	case ERangeBearState::Approach:
		if (RangeBear->CanThrowBeeNest())
		{
			RangeBear->StartAttacking();
			ChangeRangeBearState(*BlackboardComponent, ERangeBearState::ThrowBeeNest);
		}
		else if (RangeBear->CanDoBaseAttack())
		{
			RangeBear->StartAttacking();
			ChangeRangeBearState(*BlackboardComponent, ERangeBearState::Bite);
		}
		break;

	case ERangeBearState::Bite:
		if (RangeBear->IsFinishedAttacking())
		{
			if (RangeBear->CanDoBaseAttack())
			{
				RangeBear->StartAttacking();
			}
			else
			{
				ChangeRangeBearState(*BlackboardComponent, ERangeBearState::Approach);
			}
		}
		break;

	case ERangeBearState::ThrowBeeNest:
		if (RangeBear->IsFinishedAttacking())
		{
			if (RangeBear->CanThrowBeeNest())
			{
				RangeBear->StartAttacking();
			}
			else
			{
				ChangeRangeBearState(*BlackboardComponent, ERangeBearState::Approach);
			}
		}
		break;
	}
}

void UBTService_CheckRangeBearState::ChangeRangeBearState(UBlackboardComponent& InBlackboardComponent, ERangeBearState InNewState) const
{
	InBlackboardComponent.SetValueAsEnum(CombatStateKey.SelectedKeyName, static_cast<uint8>(InNewState));
}
