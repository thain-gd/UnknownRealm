#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckState.generated.h"

UCLASS(Abstract)
class UNKNOWNREALM_API UBTService_CheckState : public UBTService
{
	GENERATED_BODY()

protected:
	// InNewState - must be an enum inherits from uint8
	void SetState(UBlackboardComponent& InBlackboardComponent, const uint8 InNewState) const;


protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector StateKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetPlayerKey;
};
