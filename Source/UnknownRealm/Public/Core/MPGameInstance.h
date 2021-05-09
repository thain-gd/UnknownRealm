// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MPGameInstance.generated.h"

UENUM(BlueprintType)
enum EGameplayState
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
	
	UFUNCTION(BlueprintImplementableEvent)
	void HostGame();

	UFUNCTION(BlueprintCallable)
	void StartPlaying();

	UFUNCTION(BlueprintCallable)
	void OnGameStarted();

	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void ShowServers();

	UFUNCTION(BlueprintCallable)
	bool TransitionToState(EGameplayState NewState);

private:
	bool IsCurrentState(EGameplayState NewState) const;
	void ShowWidget(UUserWidget*& Widget, TSubclassOf<UUserWidget> WidgetClass);

	void HideWidget(UUserWidget* Widget);

	

public:
	UPROPERTY(BlueprintReadOnly)
	bool bEnableLAN;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ErrorScreen;

	EGameplayState CurrentState;

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
};
