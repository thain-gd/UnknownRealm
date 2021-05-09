#include "UI/MainMenu.h"
#include "Core/MPGameInstance.h"

FText UMainMenu::GetNETText() const
{
	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (GameInstance == nullptr)
		return FText::FromString("");

	if (GameInstance->bEnableLAN)
		return FText::FromString("LAN mode");

	return FText::FromString("Internet mode");
}

void UMainMenu::OnPlayBtnClicked() const
{
	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (GameInstance)
		GameInstance->HostGame();
}

void UMainMenu::OnFindGamesBtnClicked() const
{
	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (GameInstance)
		GameInstance->ShowServers();
}

void UMainMenu::OnToggleLANBtnClicked() const
{
	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->bEnableLAN = !GameInstance->bEnableLAN;
	}
}
