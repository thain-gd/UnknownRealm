// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class APlayerCharacter;
class UProgressBar;

UCLASS()
class UNKNOWNREALM_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	const APlayerCharacter* GetOwnerPlayerChar() const { return OwnerPlayerChar; }
	void SetOwnerPlayerChar(APlayerCharacter* InOwnerPlayerChar) { OwnerPlayerChar = InOwnerPlayerChar; }
	
private:
	UFUNCTION(BlueprintPure)
	FText GetPreparingTimeText() const;

	UFUNCTION(BlueprintPure)
	ESlateVisibility GetPreparingTimeVisibility() const;

	UFUNCTION(BlueprintPure)
	float GetStaminaBarPercent() const;
	
	UFUNCTION(BlueprintPure)
	float GetVillageHealthBarPercent() const;

	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* VillageHealthBar;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> VillageClass;

	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* OwnerPlayerChar;
};
