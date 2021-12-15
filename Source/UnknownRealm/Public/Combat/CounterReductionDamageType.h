// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/BaseDamageType.h"
#include "CounterReductionDamageType.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UCounterReductionDamageType : public UBaseDamageType
{
	GENERATED_BODY()

public:
	void HandleDamage(float InDamage, AActor* InCauser, AActor* InReceiver) const override;
};
