// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/Sword.h"

#include "Components/BaseComboComponent.h"
#include "Player/PlayerCharacter.h"

ASword::ASword()
{
	WeaponType = EWeaponType::Sword;
	
	ComboComp = CreateDefaultSubobject<UBaseComboComponent>(TEXT("BaseComboComp"));
	AddOwnedComponent(ComboComp);
}

void ASword::SetupInputs(UInputComponent* ControllerInputComp)
{
	Super::SetupInputs(ControllerInputComp);

	ControllerInputComp->BindAction("CounterAttack", IE_Pressed, this, &ASword::SR_TriggerCounterAttack);
}

void ASword::SR_TriggerCounterAttack_Implementation()
{
	if (!IsReadyToDoCounterAttack())
		return;

	bCanCounterAttack = false; // Reset to prevent multiple counter attacks
	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	check(PlayerAnimInstance != nullptr);
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Play(CounterAttackMontage);
	}
}

bool ASword::IsReadyToDoCounterAttack() const
{
	// Can only do counter attack after an attack already starts and currently in recovery frame
	return IsAttacking() && bCanCounterAttack;
}
