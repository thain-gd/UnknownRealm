// Fill out your copyright notice in the Description page of Project Settings.


#include "Equips/ComboWeapon.h"

#include "GameFramework/Character.h"
#include "Player/PlayerCharacter.h"

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

	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	if (!PlayerAnimInstance)
		return;
	
	if (!PlayerAnimInstance->IsAnyMontagePlaying())
	{
		ComboCount = 1;

		MC_StartLightAttack();
	}
	else if (ComboCount < MaxComboCount)
	{
		const FString CurrentSectionName = PlayerAnimInstance->Montage_GetCurrentSection().ToString();
		if (CurrentSectionName.Contains(TEXT("ComboWindow")) && CurrentSectionName.Contains(FString::FromInt(ComboCount)))
		{
			++ComboCount;

			const FString NextAttackStr = FString::Printf(TEXT("LightAttack%d"), ComboCount);
			MC_SetNextComboAttack(*NextAttackStr);
		}
	}
}

void AComboWeapon::MC_StartLightAttack_Implementation()
{
	GetOwner<APlayerCharacter>()->PlayAnimMontage(ComboMontage, 1.0f, NAME_None);
}

void AComboWeapon::TriggerHeavyAttack()
{
}

void AComboWeapon::MC_StartHeavyAttack_Implementation()
{
	GetOwner<APlayerCharacter>()->PlayAnimMontage(ComboMontage, 1.0f, FName("HeavyAttack1"));
}

void AComboWeapon::MC_SetNextComboAttack_Implementation(const FName& NextAttackName)
{
	UAnimInstance* PlayerAnimInstance = GetOwner<APlayerCharacter>()->GetAnimInstance();
	PlayerAnimInstance->Montage_SetNextSection(PlayerAnimInstance->Montage_GetCurrentSection(), NextAttackName);
}
