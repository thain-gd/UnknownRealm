// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckRangeBearState.generated.h"

UENUM(BlueprintType)
enum class ERangeBearState : uint8
{
	Approach,
	Bite,
	ThrowBeeNest,
};

UCLASS()
class UNKNOWNREALM_API UBTService_CheckRangeBearState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckRangeBearState();

protected:
	void TickNode(UBehaviorTreeComponent& InOwnerComp, uint8* InNodeMemory, float InDeltaSeconds) override;

private:
	void ChangeRangeBearState(UBlackboardComponent& InBlackboardComponent, ERangeBearState InNewState) const;


private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector CombatStateKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector TargetPlayerKey;
};
