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

	bool IsAlive() const { return CurrentHealth > 0; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleDamageTaken(AActor* OnTakeAnyDamage, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


public:
	FOnHealthChangedDelegate OnHealthChanged;

private:
	UPROPERTY(EditDefaultsOnly)
	int32 MaxHealth;

	int32 CurrentHealth;
};
