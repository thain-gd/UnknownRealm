// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/ComboWeapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API ASword : public AComboWeapon
{
	GENERATED_BODY()

public:
	ASword();
	
	virtual void SetupInputs(UInputComponent* ControllerInputComp) override;

protected:
	virtual void OnEndOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
private:
	UFUNCTION(Server, Reliable)
	void SR_TriggerCounterAttack();

	bool IsReadyToDoCounterAttack() const;

	UFUNCTION(BlueprintCallable)
	void StartCheckingLastHAttackStep();

	UFUNCTION()
	void PlayLastHAttackStep() const;

	void StopCheckingLastHAttackStep();

	UFUNCTION(BlueprintCallable)
	void ApplyLastHAttackEffect();

	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CounterAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LastHAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TimeBeforeTriggerLastHAttackStep;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanCounterAttack;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanDoNextCounterStep;

	FTimerHandle LastHAttackStepTriggerTimerHandle;
};
