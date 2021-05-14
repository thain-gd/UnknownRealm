// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPGameController.generated.h"

class AMPPlayerController;
class AMPPlayerState;

UCLASS()
class UNKNOWNREALM_API AMPGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPGameController();

	UFUNCTION(Server, Reliable)
	void ServerAddPlayer(APlayerController* NewPlayer);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure)
	float GetRemainingTime() const
	{
		return GetWorldTimerManager().GetTimerRemaining(PreparingTimeHandle);
	}
	
private:
	void StartWave();
	
	
private:
	UPROPERTY()
	TArray<AMPPlayerController*> PlayerControllers;

	UPROPERTY()
	TArray<AMPPlayerState*> PlayerStates;

	UPROPERTY(EditAnywhere, Category = GameController)
	float MaxPreparingTime;
	
	FTimerHandle PreparingTimeHandle;
};
