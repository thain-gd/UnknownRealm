// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnHealthChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void TakeDamage(float InDamage, AActor* InCauser, bool bInShowDamage = false);

	UFUNCTION(Server, Reliable)
	void SR_IncreaseHealth(float InAmount);

	bool IsAlive() const { return CurrentHealth > 0; }

	bool GetInvincibility() const { return bIsInvincible; }
	void SetInvincibility(bool bInIsInvincible) { bIsInvincible = bInIsInvincible; }

	float GetMaxHealth() const { return MaxHealth; }
	float GetRemainingHealth() const { return CurrentHealth; }
	float GetRemainingHealthPercent() const { return CurrentHealth / MaxHealth; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleDamageTaken(AActor* InDamagedActor, float InDamage, const class UDamageType* InDamageType, class AController* InInstigatedBy, AActor* InDamageCauser);


public:
	FOnHealthChangedDelegate OnHealthChanged;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	bool bIsInvincible;
};
