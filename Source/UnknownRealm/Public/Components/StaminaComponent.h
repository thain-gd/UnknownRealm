// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	// Called every frame
	virtual void TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) override;

	bool DecreaseStaminaByPercentage(float InStaminaPercentage);
	bool DecreaseStaminaByPoint(float InStaminaCost);
	float GetCurrentStaminaPercent() const { return CurrentStamina / MaxStamina; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void RegenStamina(float InDeltaTime);
	
	bool IsEnoughStamina(float InStamina) const { return InStamina <= CurrentStamina; }
	bool IsStaminaFull() const { return CurrentStamina == MaxStamina; }

protected:
	const int32 MaxStamina = 100;
	
	UPROPERTY(EditAnywhere)
	float StaminaRegenRate;

	float CurrentStamina;

	UPROPERTY(EditAnywhere)
	float TimeBeforeRegen;
	
	float TimeSinceLastStaminaUsed;
};
