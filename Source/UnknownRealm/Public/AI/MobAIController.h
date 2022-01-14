// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MobAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AMobAIController : public AAIController
{
	GENERATED_BODY()

public:
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;


private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BaseTree;
};
