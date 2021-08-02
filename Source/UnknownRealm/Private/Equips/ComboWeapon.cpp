// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/ComboWeapon.h"

void AComboWeapon::SetupInputs(UInputComponent* ControllerInputComp)
{
	ControllerInputComp->BindAction("NormalAttack", IE_Pressed, this, &AComboWeapon::SR_TriggerLightAttack);
}

void AComboWeapon::SR_TriggerLightAttack_Implementation()
{
	if (!bIsWeaponActive)
	{
		SetIsWeaponActive(true);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("SR_TriggerLightAttack"));
}

void AComboWeapon::TriggerHeavyAttack()
{
}
