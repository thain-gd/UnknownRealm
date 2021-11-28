// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equips/Weapon.h"
#include "RangeWeapon.generated.h"

class APlayerCharacter;
class UCameraComponent;
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

	void CL_SetupInputs_Implementation() override;

	virtual void Tick(float DeltaSeconds) override;

	float GetChargeAmount() const { return ChargeAmount; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnChargingFinish();
	
private:
	UFUNCTION(Client, Reliable)
	void CL_SetupComponents(APlayerCharacter* PlayerChar);

	void SetupComponents(APlayerCharacter* PlayerChar);

	void SetupChargeTimeline();

	UFUNCTION(Server, Unreliable)
	void SR_UpdateChargeTimelineComp(float NewChargeAmount);

	UFUNCTION()
	void OR_UpdateTimingMultiplierByChargeAmount();

	UFUNCTION(BlueprintCallable)
	void UpdateTimingMultiplier(ETimingState TimingState);
	
	UFUNCTION(Server, Reliable)
	void SR_OnAimingStarted();

	UFUNCTION(BlueprintCallable)
	bool TryReload();
	void Reload();

	UFUNCTION(Server, Reliable)
	void SR_OnAimingEnded();

	void ResetArrow();

	UFUNCTION(NetMulticast, Reliable)
	void MC_UpdateIsAiming(bool bInIsAiming);

	void ShowIndicator();
	void HideIndicator() const;

	void OnChargingStart();

	void Fire();

	float CalculateDamage() const;

	UFUNCTION(Server, Reliable)
	void SR_OnFired(const FVector& InTargetLocation, const float Damage);
	
	void StopCharge() const;

	void UpdateTarget();
	void TraceHitTarget(FHitResult& OutHitResult, const FVector& StartLocation, const FVector& EndLocation) const;

	UFUNCTION(BlueprintCallable)
	void UpdateIndicatorByRange(bool bIsTargetEnemy, float CurrentRange);

protected:
	virtual int32 GetTotalDmg() const override { return LastTotalDmg; }
	

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

	UPROPERTY(ReplicatedUsing = OR_UpdateTimingMultiplierByChargeAmount, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY()
	UCameraComponent* PlayerCharCameraComp;

	FVector TargetLocation;

	FName CurrentArrowID = FName("NormalArrow"); // TODO: Update dynamically later when add more arrow types

	int32 LastTotalDmg;
	
	const float OptimalRangeMultiplier		= 1.0f;
	const float AcceptableRangeMultiplier	= 0.7f;
	const float DefaultTimingMultiplier		= 1.0f;
	const float GoodTimingMultiplier		= 1.2f;
	const float PerfectTimingMultiplier		= 1.6f;
};
