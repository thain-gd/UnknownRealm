// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ComboComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract)
class UNKNOWNREALM_API UComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComboComponent();

	// "Abstract" functions, children must override
	virtual UAnimMontage* GetNextLightAttackMontage() { return nullptr; }
	virtual UAnimMontage* GetNextHeavyAttackMontage() { return nullptr; }

	virtual void Reset();

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> HeavyAttackMontages;

	int32 LightAttackMontageIndex;
	int32 HeavyAttackMontageIndex;
};
