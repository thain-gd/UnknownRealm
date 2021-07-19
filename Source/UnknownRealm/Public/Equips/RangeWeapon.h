// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Weapon.h"
#include "RangeWeapon.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API ARangeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ARangeWeapon();

	virtual void Init(FEquipmentInfo* InEquipInfo) override;

	bool StartAiming();
	void StopAiming();

	void ShowIndicator();
	void HideIndicator() const;

	UFUNCTION(BlueprintImplementableEvent)
	void BeginCharge();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StopCharge();
	
	void Fire(const FVector& TargetLocation);

private:
	bool TryReload();
	void Reload();

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ArrowClass;
	
	UPROPERTY()
	AProjectile* Arrow;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BowWidgetClass;

	UPROPERTY()
	UUserWidget* BowWidget;
};
