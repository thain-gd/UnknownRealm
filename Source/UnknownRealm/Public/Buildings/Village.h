// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/DamageableActor.h"
#include "Village.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AVillage : public ADamageableActor
{
	GENERATED_BODY()

protected:
	virtual void OnHealthChanged() override;
};
