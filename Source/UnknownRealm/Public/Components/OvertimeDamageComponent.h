// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OvertimeDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UOvertimeDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOvertimeDamageComponent();

	void Init(AActor* InDamageCauser, float InDamage, float InDuration, float InTriggerRate);

private:
	void ApplyDamage();


private:
	UPROPERTY()
	AActor* MyDamageCauser;
	
	float MyDamage;
	int32 MyTriggerTimes;
	FTimerHandle MyApplyDamageTimerHandle;
};
