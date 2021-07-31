// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Weapon.h"
#include "RangeWeapon.generated.h"

class UTimelineComponent;
class UBowWidget;
UENUM(BlueprintType)
enum class ERangeState : uint8
{
	OutOfRange,
	Normal,
	Optimal
};

UENUM(BlueprintType)
enum class ETimingState : uint8
{
	Default,
	Good,
	Perfect
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

	UFUNCTION(Server, Unreliable)
	void ServerUpdateChargeTimelineComp(float NewChargeAmount);

	bool StartAiming();
	void StopAiming();

	void ShowIndicator();
	void HideIndicator() const;

	UFUNCTION(BlueprintCallable)
	void UpdateIndicatorByRange(bool bIsTargetEnemy, float CurrentRange);

	UFUNCTION()
	void OnRepUpdateTimingMultiplierByChargeAmount();
	
	UFUNCTION(BlueprintCallable)
	void UpdateTimingMultiplier(ETimingState TimingState);

	void OnChargingStart() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnChargingFinish();

	UFUNCTION(BlueprintImplementableEvent)
	void StartCharge();
	
	void Fire(const FVector& TargetLocation);

	float GetDamage() const;
	float GetChargeAmount() const { return ChargeAmount; }

private:
	UFUNCTION(Client, Reliable)
	void ClientSetupChargeTimeline() const;
	
	void StopCharge() const;
	
	UFUNCTION(Server, Reliable)
	void ServerOnFired(const FVector& TargetLocation, const float Damage);

	UFUNCTION(BlueprintCallable)
	bool TryReload();
	void Reload();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ArrowClass;
	
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AProjectile* Arrow;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BowWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBowWidget* BowWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* ChargeTimelineComp;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* ChargeAmountFloatCurve;

	UPROPERTY(ReplicatedUsing = OnRepUpdateTimingMultiplierByChargeAmount, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
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

	FName CurrentArrowID = FName("NormalArrow"); // TODO: Update dynamically later when add more arrow types
	
	const float OptimalRangeMultiplier		= 1.0f;
	const float AcceptableRangeMultiplier	= 0.7f;
	const float DefaultTimingMultiplier		= 1.0f;
	const float GoodTimingMultiplier		= 1.2f;
	const float PerfectTimingMultiplier		= 1.6f;
};
