// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "MPPlayerController.generated.h"

class APlayerCharacter;
class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API AMPPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	friend class UInventoryWidget;

public:
	AMPPlayerController();
	
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Client, Reliable)
	void CL_PostLogin();

	void OnRep_Pawn() override;
	
	void ShowWaveResult();

	UFUNCTION(Server, Reliable)
	void SR_DestroyActor(AActor* Actor);

	void SetInputToGameAndUI();
	void SetInputToGameOnly();

	// For AI
	FGenericTeamId GetGenericTeamId() const override { return MyTeamId; }

private:
	void SetupUIs();
	
	void ToggleGameMenu();
	void ShowGameMenu();
	void HideGameMenu();

	void ToggleInventory();
	

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameMenuClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUDWidget> HUDClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> StatusWidgetClass;

private:
	UPROPERTY()
	UUserWidget* GameMenu;
	
	UPROPERTY()
	UPlayerHUDWidget* HUD;

	UPROPERTY()
	UUserWidget* StatusWidget;

	bool bGameMenuOn;
	bool bInventoryOn;

	FGenericTeamId MyTeamId;
};
