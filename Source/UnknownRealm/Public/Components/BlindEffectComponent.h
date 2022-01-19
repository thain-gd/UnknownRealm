#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlindEffectComponent.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UBlindEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void Init();

private:
	void DecreaseRemainingRollTimes();
	void RemoveBlindEffect();

	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BlindEffectHUDClass;

	UPROPERTY()
	UUserWidget* BlindEffectHUD;

	UPROPERTY(EditDefaultsOnly)
	float LastingTime;

	UPROPERTY(EditDefaultsOnly)
	uint8 BlindRemovalRollTimes;

	FTimerHandle BlindRemovalTimerHandle;
	uint8 RemainingRollTimes;
};
