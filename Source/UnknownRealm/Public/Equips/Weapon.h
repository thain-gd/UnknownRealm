// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Equipment.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Sword,
	Spear,
	Bow,
	Staff
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FEquipmentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 BaseDmg;
};

UCLASS(Abstract)
class UNKNOWNREALM_API AWeapon : public AEquipment
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void Init(FEquipmentInfo* InEquipInfo) override;

	virtual void SetupInputs(UInputComponent* ControllerInputComp) {};

	bool IsWeaponActive() const { return bIsWeaponActive; }
	
	UFUNCTION(Server, Reliable)
	void SR_SetIsWeaponActive(bool bInIsWeaponActive);
	
	void SetIsWeaponActive(bool bInIsWeaponActive);

	UFUNCTION(BlueprintCallable)
	void SetMotionValue(float InMotionValue);

	bool IsAiming() const { return bIsAiming; }

	EWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION()
	virtual void OnEnemyHit(AActor* Enemy);
	
protected:
	virtual int32 GetTotalDmg() const;

private:
	UFUNCTION(Client, Reliable)
	void CL_ShowDmgDealt(int32 TotalDmg);

	
protected:
	UPROPERTY(Replicated, EditInstanceOnly)
	int32 BaseDmg;

	UPROPERTY(Replicated)
	bool bIsWeaponActive;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming;

	bool bIsLocallyControlled;

	UPROPERTY(BlueprintReadWrite)
	float CurrentMotionValue = 1.0f;

private:
	static const FName InactiveSocketName;
	
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
};
