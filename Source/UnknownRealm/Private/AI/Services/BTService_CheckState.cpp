#include "AI/Services/BTService_CheckState.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckState::SetState(UBlackboardComponent& InBlackboardComponent, const uint8 InNewState) const
{
	InBlackboardComponent.SetValueAsEnum(StateKey.SelectedKeyName, InNewState);
}
