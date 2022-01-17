// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckMeleeBearState.generated.h"

UENUM(BlueprintType)
enum class EMeleeBearState : uint8
{
	Approach,
	Slash,
	Run,
	Pounce,
	Chomp
};

UCLASS()
class UNKNOWNREALM_API UBTService_CheckMeleeBearState : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckMeleeBearState();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void ChangeMeleeBearState(UBlackboardComponent& InBlackboardComponent, EMeleeBearState InNewState) const;


private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CombatStateKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetPlayerKey;
};
