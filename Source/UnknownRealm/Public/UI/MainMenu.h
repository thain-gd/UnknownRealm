// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	FText GetNETText() const;

	UFUNCTION(BlueprintCallable)
	void OnPlayBtnClicked() const;

	UFUNCTION(BlueprintCallable)
	void OnFindGamesBtnClicked() const;

	UFUNCTION(BlueprintCallable)
	void OnToggleLANBtnClicked() const;
};
