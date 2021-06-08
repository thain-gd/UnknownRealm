// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MPGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AMPGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	int32 GetRemainingPreparingTime() const { return RemainingPreparingTime; }

	void InitPreparingTime(int32 MaxPreparingTime)
	{
		RemainingPreparingTime = MaxPreparingTime;
	}
	
	void DecreaseRemainingPreparingTime()
	{
		--RemainingPreparingTime;
	}

	void SetWaveStatus(bool Won);

private:
	UFUNCTION(Reliable, NetMulticast)
	void MulticastOnWaveStatusChanged() const;

	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
	int32 RemainingPreparingTime;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
	bool bIsWon;
};
