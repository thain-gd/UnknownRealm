// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CraftingComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Core/MPGameInstance.h"
#include "Core/MPGameState.h"
#include "UI/CraftingItemWidget.h"

void UCraftingWidget::Init(UCraftingComponent* InCraftingComp)
{
	CraftingComp = InCraftingComp;

	InitButtonClickedEvents();

	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		LoadTab(UseableList, GameInstance->GetCraftingUseableData(), true);
		LoadTab(TurretList, GameInstance->GetCraftingTurretData());
		LoadTab(TrapList, GameInstance->GetCraftingTrapData());
	}
}

void UCraftingWidget::InitButtonClickedEvents()
{
	UseablesBtn->OnClicked.AddDynamic(this, &UCraftingWidget::SwitchToUseableTab);
	TurretsBtn->OnClicked.AddDynamic(this, &UCraftingWidget::SwitchToTurretTab);
	TrapsBtn->OnClicked.AddDynamic(this, &UCraftingWidget::SwitchToTrapTab);
}

void UCraftingWidget::SwitchToTab(int32 TabIndex) const
{
	TabSwitcher->SetActiveWidgetIndex(TabIndex);
}

void UCraftingWidget::SwitchToUseableTab()
{
	SwitchToTab(0);
}

void UCraftingWidget::SwitchToTurretTab()
{
	SwitchToTab(1);
}

void UCraftingWidget::SwitchToTrapTab()
{
	SwitchToTab(2);
}

void UCraftingWidget::LoadTab(UVerticalBox* ListWidget, UDataTable* CraftingList, bool bIsUseable)
{
	for (auto& Row : CraftingList->GetRowMap())
	{
		FCraftingItem* CraftingItem = reinterpret_cast<FCraftingItem*>(Row.Value);
		UCraftingItemWidget* CraftingItemWidget = CreateWidget<UCraftingItemWidget>(this, CraftingItemWidgetClass);
		
		CraftingItemWidget->Init(bIsUseable, CraftingComp, Row.Key, CraftingItem);
		
		ListWidget->AddChildToVerticalBox(CraftingItemWidget);

		CraftingItemWidgets.Add(CraftingItemWidget);
	}
}

void UCraftingWidget::UpdateCraftableWidgets()
{
	TMap<FName, int32> AvailableResources;
	GetWorld()->GetGameState<AMPGameState>()->GetInventory()->GetAvailableResources(AvailableResources);
	for (UCraftingItemWidget* CraftingItemWidget : CraftingItemWidgets)
	{
		CraftingItemWidget->VerifyRequirements(AvailableResources);
	}
}

void UCraftingWidget::Show()
{
	HideCraftingGuidelines();
	AddToViewport();
}

void UCraftingWidget::Hide()
{
	ShowCraftingGuidelines();
	RemoveFromViewport();
}

void UCraftingWidget::ShowCraftingGuidelines() const
{
	CraftingMenu->SetVisibility(ESlateVisibility::Hidden);
	CraftingGuide->SetVisibility(ESlateVisibility::Visible);
}

void UCraftingWidget::HideCraftingGuidelines() const
{
	CraftingMenu->SetVisibility(ESlateVisibility::Visible);
	CraftingGuide->SetVisibility(ESlateVisibility::Hidden);
}