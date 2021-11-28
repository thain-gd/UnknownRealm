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
	virtual void OnEndOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UFUNCTION()
	void OnWeaponHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
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
	UPROPERTY(VisibleDefaultsOnly)
	UComboComponent* ComboComp; // Will be added from Blueprints for any specific combo component

	UPROPERTY()
	AActor* FirstHitEnemy;
	
private:
	UPROPERTY()
	UAnimMontage* NextAttackMontage;
};
