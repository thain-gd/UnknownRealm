// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AMPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Client, Reliable)
	void ClientPostLogin();

	void ShowWaveResult();

protected:
	UFUNCTION(BlueprintCallable)
	void SetupUIs();

private:
	void ToggleGameMenu();
	void ShowGameMenu();
	void HideGameMenu();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameMenuClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> StatusWidgetClass;

private:
	UPROPERTY()
	UUserWidget* GameMenu;
	
	UPROPERTY()
	UUserWidget* HUD;

	UPROPERTY()
	UUserWidget* StatusWidget;

	bool bGameMenuOn;
};
