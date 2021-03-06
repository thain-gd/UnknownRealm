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
	void CL_SetupInputs_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void EnableAttackCheck();

	UFUNCTION(BlueprintCallable)
	void DisableAttackCheck();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEndOverlapWeapon(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex);
	
private:
	UFUNCTION()
	void OnWeaponHit(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult);
	
	UFUNCTION(BlueprintCallable)
	void SetComboComp(UComboComponent* InCompoComp) { ComboComp = InCompoComp; }
	
	UFUNCTION(Server, Reliable)
	void SR_TriggerLightAttack();

	UFUNCTION(Server, Reliable)
	void SR_TriggerHeavyAttack();

	void SetNextAttackMontage(UAnimMontage* InNextMontage);

	UFUNCTION(BlueprintCallable)
	void TriggerNextAttack();
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();


protected:
	UPROPERTY(VisibleDefaultsOnly)
	UComboComponent* ComboComp; // Will be added from Blueprints for any specific combo component

	UPROPERTY()
	AActor* FirstHitEnemy;
	
private:
	UPROPERTY()
	UAnimMontage* NextAttackMontage;
};
