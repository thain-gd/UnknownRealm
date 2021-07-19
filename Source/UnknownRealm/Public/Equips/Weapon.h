// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Equipment.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
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
	EWeaponType WeaponType;

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init(FEquipmentInfo* InEquipInfo) override;

	virtual FName GetAttachPoint() const override;

	EWeaponType GetWeaponType() const { return WeaponType; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnNormalAttackPressed();
	void OnHeavyAttackPressed();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	int32 BaseDmg;
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	EWeaponType WeaponType;
};
