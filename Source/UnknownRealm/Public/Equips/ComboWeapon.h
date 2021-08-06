// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Weapon.h"
#include "ComboWeapon.generated.h"

class UComboComponent;
/**
 * This class is used for weapons with any combo pattern b/w
 * light and heavy attacks.
 * 
 */
UCLASS()
class UNKNOWNREALM_API AComboWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void SetupInputs(UInputComponent* ControllerInputComp) override;
	
private:
	UFUNCTION(BlueprintCallable)
	void SetComboComp(UComboComponent* InCompoComp) { ComboComp = InCompoComp; }
	
	UFUNCTION(Server, Reliable)
	void SR_TriggerLightAttack();

	UFUNCTION(Server, Reliable)
	void SR_TriggerHeavyAttack();

	UFUNCTION(BlueprintCallable)
	void TriggerNextAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_TriggerAttack(UAnimMontage* AttackMontage);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();


protected:
	UPROPERTY()
	UComboComponent* ComboComp;
	
private:
	UPROPERTY()
	UAnimMontage* NextAttackMontage;
};
