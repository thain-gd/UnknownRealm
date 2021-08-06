// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboComponent.h"
#include "BaseComboComponent.generated.h"

enum class EComboState : uint8
{
	NotAttack,
	LightAttack,
	HeavyAttack,
};

/**
 *	This class is used for any weapon with base compo pattern,
 *	light to heavy but not the opposite, e.g., a>b>c, d>e>f,
 *	a>d>e>f, a>b>e>f, a>b>c>f
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNKNOWNREALM_API UBaseComboComponent : public UComboComponent
{
	GENERATED_BODY()

public:
	virtual UAnimMontage* GetNextLightAttackMontage() override;
	virtual UAnimMontage* GetNextHeavyAttackMontage() override;

	virtual void Reset() override;

private:
	bool bHasHeavyAttackStarted;
	EComboState CurrComboState = EComboState::NotAttack;
};
