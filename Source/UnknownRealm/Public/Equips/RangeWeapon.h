// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Weapon.h"
#include "RangeWeapon.generated.h"

UENUM(BlueprintType)
enum class ERangeState : uint8
{
	OutOfRange,
	Normal,
	Optimal
};

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

	UFUNCTION(BlueprintCallable)
	void UpdateIndicatorByRange(bool bIsTargetEnemy, float CurrentRange);

	UFUNCTION(BlueprintImplementableEvent)
	void BeginCharge();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StopCharge();
	
	void Fire(const FVector& TargetLocation);

	float GetDamage() const;
	float GetChargeAmount() const { return ChargeAmount; }

protected:
	UFUNCTION()
	virtual void OnRepSetMesh() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBowWidget(ERangeState RangeState);

private:
	UFUNCTION(Server, Reliable)
	void ServerOnFired(const FVector& TargetLocation, const float Damage);
	
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

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* BowWidget;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ChargeAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float OptimalRangeUpperBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float OptimalRangeLowerBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxRange;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TimingMultiplier;

	float RangeMultiplier;
	
	const float OptimalRangeMultiplier = 1.0f;
	const float AcceptableRangeMultiplier = 0.7f;
	const float DefaultTimingMultiplier = 1.0f;
};
