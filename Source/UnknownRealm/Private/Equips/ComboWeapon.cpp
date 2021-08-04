// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/ComboWeapon.h"

#include "GameFramework/Character.h"

void AComboWeapon::SetupInputs(UInputComponent* ControllerInputComp)
{
	ControllerInputComp->BindAction("LightAttack", IE_Pressed, this, &AComboWeapon::SR_TriggerLightAttack);
}

void AComboWeapon::SR_TriggerLightAttack_Implementation()
{
	if (!bIsWeaponActive)
	{
		SetIsWeaponActive(true);
		return;
	}
	
	MC_TriggerLightAttack();
}

void AComboWeapon::MC_TriggerLightAttack_Implementation()
{
	if (ComboCount >= MaxComboCount)
		return;

	++ComboCount;
	const FString AttackSectionName = FString::Printf(TEXT("LightAttack%d"), ComboCount);
	UE_LOG(LogTemp, Warning, TEXT("Trigger %s"), *AttackSectionName);
	GetOwner<ACharacter>()->PlayAnimMontage(ComboMontage, 1.0f, *AttackSectionName);
}

void AComboWeapon::TriggerHeavyAttack()
{
}
