// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckBaseState.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UBTService_CheckBaseState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckBaseState();

protected:
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
private:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector CurrentStateKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetPlayerKey;
};
