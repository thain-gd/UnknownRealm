// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_CheckBaseState.h"

#include "AIController.h"
#include "AI/AIChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/PlayerCharacter.h"

UBTService_CheckBaseState::UBTService_CheckBaseState()
{
	NodeName = TEXT("Check AI Current State");
	bNotifyBecomeRelevant = true;
}

void UBTService_CheckBaseState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(CurrentStateKey.SelectedKeyName, static_cast<uint8>(EAIState::ApproachVillage));
}

void UBTService_CheckBaseState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIChar* AIChar = OwnerComp.GetAIOwner()->GetPawn<AAIChar>();
	if (!AIChar)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const EAIState CurrentState = static_cast<EAIState>(BlackboardComponent->GetValueAsEnum(CurrentStateKey.SelectedKeyName));
	switch (CurrentState)
	{
	case EAIState::AttackVillage:
		if (AIChar->HasAttackablePlayer())
		{
			APlayerCharacter* TargetPlayer = AIChar->RemoveFirstTargetPlayer();
			BlackboardComponent->SetValueAsObject(TargetPlayerKey.SelectedKeyName, TargetPlayer);

			BlackboardComponent->SetValueAsEnum(CurrentStateKey.SelectedKeyName, static_cast<uint8>(EAIState::EncounterPlayer));
		}
		break;

	case EAIState::EncounterPlayer:
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(BlackboardComponent->GetValueAsObject(TargetPlayerKey.SelectedKeyName)))
		{
			if (PlayerCharacter->IsDead())
			{
				BlackboardComponent->ClearValue(TargetPlayerKey.SelectedKeyName);
				BlackboardComponent->SetValueAsEnum(CurrentStateKey.SelectedKeyName, static_cast<uint8>(EAIState::ApproachVillage));
			}
		}
		break;

	default:
		break;
	}
}
