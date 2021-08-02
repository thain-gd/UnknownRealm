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

	bool IsAiming() const { return bIsAiming; }

	EWeaponType GetWeaponType() const { return WeaponType; }

	
protected:
	UPROPERTY(Replicated, EditInstanceOnly)
	int32 BaseDmg;

	UPROPERTY(Replicated)
	bool bIsWeaponActive;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming;

	bool bIsLocallyControlled;

private:
	static const FName InactiveSocketName;
	
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
};
