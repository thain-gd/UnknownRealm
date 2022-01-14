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

	UFUNCTION(Client, Reliable)
	virtual void CL_SetupInputs();

	UFUNCTION(Client, Reliable)
	void CL_EnableInput(APlayerController* InPlayerController);

	UFUNCTION(Client, Reliable)
	void CL_DisableInput(APlayerController* InPlayerController);
	
	bool IsWeaponActive() const { return bIsWeaponActive; }
	
	UFUNCTION(Server, Reliable)
	void SR_SetIsWeaponActive(bool bInIsWeaponActive);
	
	void SetIsWeaponActive(bool bInIsWeaponActive);

	UFUNCTION(BlueprintCallable)
	void SetMotionValue(float InMotionValue);

	bool IsAiming() const { return bIsAiming; }

	EWeaponType GetWeaponType() const { return WeaponType; }

	bool IsAttacking() const { return bIsAiming || bIsAttacking; }

	float GetSideStepStaminaPercent() const { return SideStepStaminaPercent; }

	UFUNCTION()
	virtual void OnEnemyHit(AActor* InEnemy);

	UAnimMontage* GetLeftSideStepMontage() const { return LeftSideStepMontage; }
	UAnimMontage* GetRightSideStepMontage() const { return RightSideStepMontage; }
	
protected:
	virtual int32 GetTotalDmg() const;

private:
	UFUNCTION(Client, Reliable)
	void CL_ShowDmgDealt(int32 InTotalDmg);

	
protected:
	UPROPERTY(Replicated, EditInstanceOnly)
	int32 BaseDmg;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Side Step")
	UAnimMontage* LeftSideStepMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Side Step")
	UAnimMontage* RightSideStepMontage;

	UPROPERTY(Replicated)
	bool bIsWeaponActive;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming;

	bool bIsLocallyControlled;

	UPROPERTY(BlueprintReadWrite)
	float CurrentMotionValue = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float SideStepStaminaPercent = 0.3f;


protected:
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
	
private:
	static const FName InactiveSocketName;
};
