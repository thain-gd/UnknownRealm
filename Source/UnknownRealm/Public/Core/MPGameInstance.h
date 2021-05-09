// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MPGameInstance.generated.h"

UENUM()
enum class GameplayState
{
	Startup,
	MainMenu,
	ServerList,
	LoadingScreen,
	ErrorDialog,
	GameInProgress,
	Indeterminate
};

/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UMPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMPGameInstance();
	
	UFUNCTION(BlueprintCallable)
	void HostGame();

	UFUNCTION(BlueprintCallable)
	void ShowServers();

private:
	void ShowLoadingScreen();
	bool TransitionToState(GameplayState NewState);
	bool IsCurrentState(GameplayState NewState) const;

	static void HideWidget(UUserWidget* Widget);

	

public:
	bool bEnableLAN;
	GameplayState CurrentState;

private:
	UPROPERTY()
	UUserWidget* MainMenu;
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	UUserWidget* ServerList;
	TSubclassOf<UUserWidget> ServerListClass;

	UPROPERTY()
	UUserWidget* LoadingScreen;
	TSubclassOf<UUserWidget> LoadingScreenClass;

	UPROPERTY()
	UUserWidget* ErrorScreen;
	TSubclassOf<UUserWidget> ErrorScreenClass;
};
