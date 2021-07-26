// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
	: StaminaRegenRate(25.0f), TimeBeforeRegen(1.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentStamina = MaxStamina;
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastStaminaUsed += DeltaTime;
	if (IsStaminaFull() || TimeSinceLastStaminaUsed < TimeBeforeRegen)
		return;

	RegenStamina(DeltaTime);
}

void UStaminaComponent::RegenStamina(float DeltaTime)
{
	CurrentStamina = FMath::Min(CurrentStamina + StaminaRegenRate * DeltaTime, (float)MaxStamina);

	// TODO: Cancel warning animation for the stamina bar
}

bool UStaminaComponent::DecreaseStamina(float UsedStamina)
{
	if (!IsEnoughStamina(UsedStamina))
		return false;

	CurrentStamina -= UsedStamina;

	TimeSinceLastStaminaUsed = 0;
	// TODO: Show warning animation for the stamina bar if lower than 25-30%
	return true;
}

