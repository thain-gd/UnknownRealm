// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UMPGameInstance::UMPGameInstance()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/UI_MainMenu"));
	if (MainMenuBPClass.Class)
		MainMenuClass = MainMenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> ServerListBPClass(TEXT("/Game/UI/UI_ServerList"));
	if (ServerListBPClass.Class)
		ServerListClass = ServerListBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenBPClass(TEXT("/Game/UI/UI_LoadingScreen"));
	if (LoadingScreenBPClass.Class)
		LoadingScreenClass = LoadingScreenBPClass.Class;
}

void UMPGameInstance::ShowServers()
{
	if (!TransitionToState(EGameplayState::ServerList))
		return;

	ShowWidget(ServerList, ServerListClass);
}

void UMPGameInstance::StartPlaying()
{
	TransitionToState(EGameplayState::GameInProgress);
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

bool UMPGameInstance::IsCurrentState(EGameplayState NewState) const
{
	return CurrentState == NewState;
}

void UMPGameInstance::ShowWidget(UUserWidget*& Widget, TSubclassOf<UUserWidget> WidgetClass)
{
	if (!Widget)
	{
		Widget = CreateWidget<UUserWidget>(this, WidgetClass);
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		const FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		Widget->AddToViewport();
	}
}

void UMPGameInstance::HideWidget(UUserWidget* Widget)
{
	if (Widget)
		Widget->RemoveFromParent();
}
