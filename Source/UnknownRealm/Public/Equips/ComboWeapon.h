// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Weapon.h"
#include "ComboWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AComboWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void SetupInputs(UInputComponent* ControllerInputComp) override;
	
private:
	UFUNCTION(Server, Reliable)
	void SR_TriggerLightAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MC_TriggerLightAttack();
	
	void TriggerHeavyAttack();


protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ComboMontage;
	
private:
	static const int32 MaxComboCount = 3;
	
	int32 ComboCount;
};
