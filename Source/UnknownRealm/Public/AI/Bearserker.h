// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIChar.h"
#include "Bearserker.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API ABearserker : public AAIChar
{
	GENERATED_BODY()

public:
	ABearserker();

	UFUNCTION(NetMulticast, Reliable)
	void MC_StartCrawling();

	UFUNCTION(NetMulticast, Reliable)
	void MC_StopCrawling();


protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsCrawling;
};
