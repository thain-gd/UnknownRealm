// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "MPGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameplayState : uint8
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

	UDataTable* GetInventoryItemData() const { return InventoryItemData; }
	UDataTable* GetCraftingUseableData() const { return CraftingUseableData; }
	UDataTable* GetCraftingTurretData() const { return CraftingTurretData; }
	UDataTable* GetCraftingTrapData() const { return CraftingTrapData; }

private:
	bool IsCurrentState(EGameplayState NewState) const;
	void ShowWidget(UUserWidget*& Widget, TSubclassOf<UUserWidget> WidgetClass);

	void HideWidget(UUserWidget* Widget);

	

public:
	UPROPERTY(BlueprintReadOnly)
	bool bEnableLAN;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ErrorScreen;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ServerListClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingScreenClass;

	EGameplayState CurrentState;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* InventoryItemData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* CraftingUseableData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* CraftingTurretData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* CraftingTrapData;
	
	UPROPERTY()
	UUserWidget* MainMenu;

	UPROPERTY()
	UUserWidget* ServerList;

	UPROPERTY()
	UUserWidget* LoadingScreen;
};
