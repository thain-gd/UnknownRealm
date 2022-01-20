// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Core/MPGameState.h"
#include "Player/PlayerCharacter.h"
#include "GameTeam.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUDWidget.h"

void AMPPlayerController::CL_PostLogin_Implementation()
{
	SetupUIs();
}

void AMPPlayerController::SetupUIs()
{
	HUD = CreateWidget<UPlayerHUDWidget>(this, HUDClass);
	HUD->SetOwnerPlayerChar(GetPawn<APlayerCharacter>());
	HUD->AddToViewport();

	GameMenu = CreateWidget(this, GameMenuClass);

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(true);
	SetInputMode(InputMode);

	bShowMouseCursor = false;
}

AMPPlayerController::AMPPlayerController()
{
	MyTeamId = FGenericTeamId(EGameTeam::Player);
}

void AMPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OpenGameMenu", IE_Pressed, this, &AMPPlayerController::ToggleGameMenu);
	InputComponent->BindAction("Inventory", IE_Pressed, this, &AMPPlayerController::ToggleInventory);
}

void AMPPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (IsValid(HUD))
		HUD->RemoveFromParent();

	if (IsValid(StatusWidget))
		StatusWidget->RemoveFromParent();
}

void AMPPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	// Clients' Pawns will be nullptr at the time SetupUIs is called so need to set it here
	if (GetPawn() && HUD && !HUD->GetOwnerPlayerChar())
	{
		HUD->SetOwnerPlayerChar(GetPawn<APlayerCharacter>());
	}
}

void AMPPlayerController::ShowWaveResult()
{
	CreateWidget(this, StatusWidgetClass)->AddToViewport();
}

void AMPPlayerController::SR_DestroyActor_Implementation(AActor* Actor)
{
	GetWorld()->DestroyActor(Actor);
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
	SetInputToGameAndUI();

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn());
	if (PlayerChar)
		PlayerChar->DisableInput(this);
}

void AMPPlayerController::HideGameMenu()
{
	GameMenu->RemoveFromViewport();
	SetInputToGameOnly();
	
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn());
	if (PlayerChar)
		PlayerChar->EnableInput(this);
}

void AMPPlayerController::SetInputToGameAndUI()
{
	bShowMouseCursor = true;

	const FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void AMPPlayerController::SetInputToGameOnly()
{
	bShowMouseCursor = false;

	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMPPlayerController::ToggleInventory()
{
	AMPGameState* GameState = Cast<AMPGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		// Close inventory
		if (bInventoryOn)
		{
			GameState->CloseInventory();
			SetInputToGameOnly();
			bInventoryOn = false;
		}
		// Open inventory
		else
		{
			GameState->OpenInventory();
			SetInputToGameAndUI();
			bInventoryOn = true;
		}
	}
}
