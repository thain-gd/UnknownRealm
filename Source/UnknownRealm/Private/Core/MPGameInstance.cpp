// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Components/CraftingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/NotifierWidget.h"

void UMPGameInstance::ShowServers()
{
	if (!TransitionToState(EGameplayState::ServerList))
		return;

	ShowWidget(ServerList, ServerListClass);
}

void UMPGameInstance::StartPlaying()
{
	TransitionToState(EGameplayState::GameInProgress);

	if (!NotifierWidget)
	{
		NotifierWidget = CreateWidget<UNotifierWidget>(this, NotifierWidgetClass);
	}
}

void UMPGameInstance::OnGameStarted()
{
	if (IsCurrentState(EGameplayState::Startup))
		ShowMainMenu();
}

void UMPGameInstance::ShowLoadingScreen()
{
	if (!TransitionToState(EGameplayState::LoadingScreen))
		return;

	ShowWidget(LoadingScreen, LoadingScreenClass);
}

void UMPGameInstance::ShowMainMenu()
{
	if (IsCurrentState(EGameplayState::GameInProgress))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartMenu"));
		FLatentActionInfo LatentInfo;
		UKismetSystemLibrary::Delay(GetWorld(), 0.2f, LatentInfo);
	}

	if (TransitionToState(EGameplayState::MainMenu))
	{
		ShowWidget(MainMenu, MainMenuClass);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PlayerController))
			PlayerController->bShowMouseCursor = true;
	}
}

bool UMPGameInstance::TransitionToState(EGameplayState NewState)
{
	if (IsCurrentState(NewState))
		return false;

	switch (CurrentState)
	{
	case EGameplayState::Startup:
		break;

	case EGameplayState::MainMenu:
		HideWidget(MainMenu);
		break;

	case EGameplayState::ServerList:
		HideWidget(ServerList);
		break;

	case EGameplayState::LoadingScreen:
		HideWidget(LoadingScreen);
		break;

	case EGameplayState::ErrorDialog:
		HideWidget(ErrorScreen);
		break;

	case EGameplayState::GameInProgress:
	case EGameplayState::Indeterminate:
		break;
	}

	CurrentState = NewState;
	return true;
}

void UMPGameInstance::ShowDamage(int32 Damage, bool bIsCrit) const
{
	NotifierWidget->ShowDamage(Damage, bIsCrit);
}

FCraftingItem* UMPGameInstance::GetCraftingDataRow(const FName& ID) const
{
	FCraftingItem* CraftingItem = CraftingUseableData->FindRow<FCraftingItem>(ID, TEXT("UMPGameInstance::GetCraftingDataRow"));
	if (CraftingItem)
		return CraftingItem;

	CraftingItem = CraftingTurretData->FindRow<FCraftingItem>(ID, TEXT("UMPGameInstance::GetCraftingDataRow"));
	if (CraftingItem)
		return CraftingItem;

	return CraftingTrapData->FindRow<FCraftingItem>(ID, TEXT("UMPGameInstance::GetCraftingDataRow"));
}

bool UMPGameInstance::IsCurrentState(EGameplayState NewState) const
{
	return CurrentState == NewState;
}

void UMPGameInstance::ShowWidget(UUserWidget*& Widget, TSubclassOf<UUserWidget> WidgetClass)
{
	if (!Widget)
	{
		Widget = CreateWidget(this, WidgetClass);
	}

	Widget->AddToViewport();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		const FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void UMPGameInstance::HideWidget(UUserWidget* Widget)
{
	if (Widget)
		Widget->RemoveFromParent();
}
