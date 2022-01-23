// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIChar.h"
#include "MeleeBear.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AMeleeBear : public AAIChar
{
	GENERATED_BODY()

public:
	AMeleeBear();
	
	UFUNCTION(BlueprintCallable)
	void OnPounceAttackFinished();

	void AllowNextPounceAttack();

	bool CanRun() const;
	bool CanDoPounceAttack() const;

	void StartRunning() const;
	
	APlayerCharacter* PickNextTargetPlayer() override;
	bool IsChomping() const { return bIsChomping; }

protected:
	void OnGotHit(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser) override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PounceAttackCooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PounceAttackRange;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsPounceAttackReady;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsChomping;
	
	const float RunSpeed = 450.0f;
};
