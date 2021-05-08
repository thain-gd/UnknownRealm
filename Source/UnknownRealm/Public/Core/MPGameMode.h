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
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UFUNCTION(Client, Reliable)
	void ClientRespawnPlayer(APlayerController* PlayerController);

	bool GetGameController();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnCharClass;

private:
	UPROPERTY()
	AMPGameController* GameController;
	
	uint32 SpawnPlayerCount;
	TArray<APlayerStart*> StartLocations;

};
