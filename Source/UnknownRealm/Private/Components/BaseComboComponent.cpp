// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseComboComponent.h"

UAnimMontage* UBaseComboComponent::GetNextLightAttackMontage()
{
	switch (CurrComboState)
	{
	case EComboState::NotAttack:
		CurrComboState = EComboState::LightAttack;
		break;
		
	case EComboState::HeavyAttack: // Not allow heavy to light
		return nullptr;

	case EComboState::LightAttack:
		++LightAttackMontageIndex;
		// Prevent light attack to continue after reached the final light attack
		if (LightAttackMontageIndex == LightAttackMontages.Num())
			return nullptr;
		
		break;
	}
	
	return LightAttackMontages[LightAttackMontageIndex];
}

UAnimMontage* UBaseComboComponent::GetNextHeavyAttackMontage()
{
	switch (CurrComboState)
	{
	case EComboState::NotAttack:
		CurrComboState = EComboState::HeavyAttack;
		break;

	case EComboState::HeavyAttack:
		++LightAttackMontageIndex;
		break;

	case EComboState::LightAttack:
		// Match the continuous heavy index with corresponding light index
		HeavyAttackMontageIndex = LightAttackMontageIndex;

		// Prevent those with number of heavy attacks less than light attacks
		if (HeavyAttackMontageIndex >= HeavyAttackMontages.Num())
			return nullptr;
		
		break;
	}

	CurrComboState = EComboState::HeavyAttack;
	UAnimMontage* NextHeavyAttackMontage = HeavyAttackMontages[HeavyAttackMontageIndex];
	HeavyAttackMontageIndex = (HeavyAttackMontageIndex + 1) % HeavyAttackMontages.Num();
	return NextHeavyAttackMontage;
}

void UBaseComboComponent::Reset()
{
	Super::Reset();

	CurrComboState = EComboState::NotAttack;
}
