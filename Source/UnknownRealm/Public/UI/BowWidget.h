// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowWidget.generated.h"

class UImage;
enum class ERangeState : uint8;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UBowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartCharging();

	UFUNCTION(BlueprintCallable)
	void StopCharging();

	UFUNCTION(BlueprintCallable)
	void UpdateCrosshair(ERangeState RangeState);

private:
	UFUNCTION(BlueprintCallable)
	void ResetCrosshair();

protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Charging;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Crosshair;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* TimingRadius;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* OutOfRangeCrosshairTexture;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* NormalRangeCrosshairTexture;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* OptimalRangeCrosshairTexture;

private:
	ERangeState CurrRangeState;
};
