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

UCLASS()
class UNKNOWNREALM_API AWeapon : public AEquipment
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void Init(FEquipmentInfo* InEquipInfo) override;

	EWeaponType GetWeaponType() const { return WeaponType; }

private:
	void OnNormalAttackPressed();
	void OnHeavyAttackPressed();
	
protected:
	UPROPERTY(Replicated, EditInstanceOnly)
	int32 BaseDmg;
	
private:
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
};
