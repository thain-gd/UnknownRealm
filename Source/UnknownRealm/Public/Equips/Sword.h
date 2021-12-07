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
	
	void CL_SetupInputs_Implementation() override;

	void AllowNextCounterStep() { bCanDoNextCounterStep = true; }

protected:
	virtual void OnEndOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
private:
	UFUNCTION(Server, Reliable)
	void SR_TriggerCounterAttack();

	bool IsReadyToDoCounterAttack() const;

	UFUNCTION(BlueprintCallable)
	void StartCheckingFinalHeavyAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayCounterAttackMontage() const;

	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayLastHAttackStep() const;

	void StopCheckingLastHAttackStep();

	UFUNCTION(BlueprintCallable)
	void ApplyFinalHeavyAttackEffect();

	void ApplyBleedingEffect(AActor* InDamageReceiver) const;

	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CounterAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LastHAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "Final Heavy Attack Trigger Time"))
	float MyFinalHeavyAttackTriggerTime;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanCounterAttack;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanDoNextCounterStep;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float BleedingDamageModifier;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float BleedingDuration;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float BleedingTriggerRate;

	FTimerHandle LastHAttackStepTriggerTimerHandle;
};
