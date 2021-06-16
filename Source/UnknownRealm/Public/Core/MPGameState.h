// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/InventoryComponent.h"
#include "GameFramework/GameState.h"
#include "MPGameState.generated.h"

struct FInventoryItem;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AMPGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMPGameState();
	
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

	void AddToInventory(const FName& ItemID, const int32 Amount);

	UInventoryComponent* GetInventory() const { return InventoryComp; }

private:
	UFUNCTION(Reliable, NetMulticast)
	void MulticastOnWaveStatusChanged() const;

	
protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
	int32 RemainingPreparingTime;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = GameState)
	bool bIsWon;

	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly)
	UInventoryComponent* InventoryComp;
};
