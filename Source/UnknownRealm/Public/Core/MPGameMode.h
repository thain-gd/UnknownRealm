#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MPGameMode.generated.h"

class APlayerStart;
class AMPGameController;

UCLASS()
class UNKNOWNREALM_API AMPGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMPGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Client, Reliable)
	void ClientRespawnPlayer(APlayerController* PlayerController);

	void InitSpawnLocations();
	void UpdateRemainingTime();
	
	void StartWave();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnCharClass;

private:
	UPROPERTY()
	TArray<APlayerStart*> SpawnLocations;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxPreparingTime; // in seconds
	
	uint32 SpawnedPlayerCount;
	FTimerHandle PreparingTimeHandle;
};
