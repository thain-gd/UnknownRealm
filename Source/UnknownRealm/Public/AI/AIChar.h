// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "AIChar.generated.h"

class UHealthComponent;

UCLASS()
class UNKNOWNREALM_API AAIChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIChar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHealthChanged();
	
	void Die();
	

protected:	
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UHealthComponent* HealthComp;

};
