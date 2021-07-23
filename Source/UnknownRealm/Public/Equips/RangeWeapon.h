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

	float GetChargeAmount() const { return ChargeAmount; }

protected:
	UFUNCTION()
	virtual void OnRepSetMesh() override;

private:
	UFUNCTION(BlueprintCallable)
	bool TryReload();
	void Reload();

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(ReplicatedUsing = OnRepSetMesh)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ArrowClass;
	
	UPROPERTY()
	AProjectile* Arrow;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BowWidgetClass;

	UPROPERTY()
	UUserWidget* BowWidget;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ChargeAmount;
};
