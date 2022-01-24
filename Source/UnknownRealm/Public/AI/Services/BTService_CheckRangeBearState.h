// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_CheckState.h"
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
class UNKNOWNREALM_API UBTService_CheckRangeBearState : public UBTService_CheckState
{
	GENERATED_BODY()

public:
	UBTService_CheckRangeBearState();

protected:
	void TickNode(UBehaviorTreeComponent& InOwnerComp, uint8* InNodeMemory, float InDeltaSeconds) override;
};
