// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingWidget.h"

#include "Components/Button.h"
#include "Components/CraftingComponent.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Core/MPGameInstance.h"
#include "UI/CraftingItemWidget.h"

void UCraftingWidget::Init(UCraftingComponent* Owner)
{
	CraftingComp = Owner;

	InitButtonClickedEvents();

	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		LoadTab(UseableList, GameInstance->GetCraftingUseableData());
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

void UCraftingWidget::LoadTab(UVerticalBox* ListWidget, UDataTable* CraftingList)
{
	TArray<FCraftingItem*> CraftingItems;
	CraftingList->GetAllRows(TEXT("UCraftingWidget::LoadUseableTab"), CraftingItems);
	for (auto CraftingItem : CraftingItems)
	{
		UCraftingItemWidget* CraftingItemWidget = CreateWidget<UCraftingItemWidget>(this, CraftingItemWidgetClass);
		CraftingItemWidget->Init(this, CraftingItem);
		
		ListWidget->AddChildToVerticalBox(CraftingItemWidget);

		CraftingItemWidgets.Add(CraftingItemWidget);
	}
}

void UCraftingWidget::UpdateCraftableWidgets(const TMap<FName, int32>& AvailableResources)
{
	for (UCraftingItemWidget* CraftingItemWidget : CraftingItemWidgets)
	{
		FCraftingItem* CraftingItemSettings = CraftingItemWidget->GetCraftingItemSettings();
		uint32 RequirementIndex = 0;
		for (auto Requirement : CraftingItemSettings->Requirements)
		{
			if (!AvailableResources.Contains(Requirement.Key) || AvailableResources[Requirement.Key] < Requirement.Value)
			{
				CraftingItemWidget->SetRequirementStatus(RequirementIndex, false);
			}
			else
			{
				CraftingItemWidget->SetRequirementStatus(RequirementIndex, true);
			}

			++RequirementIndex;
		}
	}
}

void UCraftingWidget::StartCrafting(const FCraftingItem* CraftingItemSettings)
{
	CraftingComp->ToggleWidget(); // Hide crafting menu

	
}
