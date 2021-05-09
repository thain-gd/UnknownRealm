// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MPGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UMPGameInstance::UMPGameInstance()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/UI/UI_MainMenu"));
	if (MainMenuBPClass.Class)
		MainMenuClass = MainMenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> ServerListBPClass(TEXT("/UI/UI_ServerList"));
	if (ServerListBPClass.Class)
		ServerListClass = ServerListBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenBPClass(TEXT("/UI/UI_LoadingScreen"));
	if (LoadingScreenBPClass.Class)
		LoadingScreenClass = LoadingScreenBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> ErrorScreenBPClass(TEXT("/UI/UI_ErrorScreen"));
	if (ErrorScreenBPClass.Class)
		ErrorScreenClass = ErrorScreenBPClass.Class;
}

void UMPGameInstance::HostGame()
{
	ShowLoadingScreen();
}

void UMPGameInstance::ShowServers()
{
}

void UMPGameInstance::ShowLoadingScreen()
{
	if (!TransitionToState(GameplayState::LoadingScreen))
		return;

	if (!LoadingScreen)
	{
		LoadingScreen = CreateWidget<UUserWidget>(this, LoadingScreenClass);
	}

	const FInputModeUIOnly InputMode;
	GetPrimaryPlayerController()->SetInputMode(InputMode);
	LoadingScreen->AddToViewport();
}

bool UMPGameInstance::TransitionToState(GameplayState NewState)
{
	if (IsCurrentState(NewState))
		return false;

	switch (CurrentState)
	{
	case GameplayState::Startup:
		break;

	case GameplayState::MainMenu:
		HideWidget(MainMenu);
		break;

	case GameplayState::ServerList:
		HideWidget(ServerList);
		break;

	case GameplayState::LoadingScreen:
		HideWidget(LoadingScreen);
		break;

	case GameplayState::ErrorDialog:
		HideWidget(ErrorScreen);
		break;

	case GameplayState::GameInProgress:
	case GameplayState::Indeterminate:
		break;
	}

	CurrentState = NewState;
	return true;
}

bool UMPGameInstance::IsCurrentState(GameplayState NewState) const
{
	return CurrentState == NewState;
}

void UMPGameInstance::HideWidget(UUserWidget* Widget)
{
	Widget->RemoveFromParent();
}
