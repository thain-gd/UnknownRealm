// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Player/PlayerCharacter.h"

void AMPPlayerController::ClientPostLogin_Implementation()
{
	SetupUIs();
}

void AMPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OpenGameMenu", IE_Pressed, this, &AMPPlayerController::ToggleGameMenu);
}

void AMPPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (IsValid(HUD))
		HUD->RemoveFromParent();

	if (IsValid(StatusWidget))
		StatusWidget->RemoveFromParent();
}

void AMPPlayerController::ShowWaveResult() const
{
	StatusWidget->AddToViewport();
}

void AMPPlayerController::SetupUIs()
{
	if (!IsLocalPlayerController())
		return;

	HUD = CreateWidget(this, HUDClass);
	HUD->AddToViewport();

	StatusWidget = CreateWidget(this, StatusWidgetClass);

	GameMenu = CreateWidget(this, GameMenuClass);

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(true);
	SetInputMode(InputMode);
	
	bShowMouseCursor = false;
}

void AMPPlayerController::ToggleGameMenu()
{
	if (!IsLocalPlayerController())
		return;
	
	if (bGameMenuOn)
	{
		HideGameMenu();
		bGameMenuOn = false;
	}
	else
	{
		ShowGameMenu();
		bGameMenuOn = true;
	}
}

void AMPPlayerController::ShowGameMenu()
{
	GameMenu->AddToViewport();

	bShowMouseCursor = true;

	const FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn());
	if (PlayerChar)
		PlayerChar->DisableInput(this);
}

void AMPPlayerController::HideGameMenu()
{
	GameMenu->RemoveFromViewport();
	
	bShowMouseCursor = false;

	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn());
	if (PlayerChar)
		PlayerChar->EnableInput(this);
}
