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

public:
	AVillage();
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	FVector GetClosestTargetableLocation(const FVector& InAILocation);

	UFUNCTION(BlueprintCallable)
	bool IsAnyTargetableLocationAvailable() const { return MyTargetableLocations.Num() > 0; }
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnHealthChanged() override;

	
private:
	UPROPERTY(EditAnywhere)
	bool bMyIsCircleBase;

	UPROPERTY(EditAnywhere)
	int32 MyTargetableLocationCount;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> MyTargetableLocations;
};
