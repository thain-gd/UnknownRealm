// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BaseDamageType.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UBaseDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	virtual void HandleDamage(float InDamage, AActor* InCauser, AActor* InReceiver) const;
};
