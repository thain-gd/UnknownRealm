// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIChar.h"
#include "RangeBear.generated.h"

class ABeeNest;

UCLASS()
class UNKNOWNREALM_API ARangeBear : public AAIChar
{
	GENERATED_BODY()

public:
	ARangeBear();
	
	bool CanThrowBeeNest() const;

private:
	UFUNCTION(BlueprintCallable)
	void SpawnBeeNest();

	UFUNCTION(BlueprintCallable)
	void ThrowBeeNest();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ThrowAttackUpperBound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ThrowAttackLowerBound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<ABeeNest> BeeNestClass;

	UPROPERTY()
	ABeeNest* BeeNest;

	FName BeeNestSocketName;
};
