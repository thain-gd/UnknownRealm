// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingWidget.h"

#include "Components/Button.h"
#include "Components/CraftingComponent.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/CraftingItemWidget.h"

void UCraftingWidget::Init(UCraftingComponent* Owner)
{
	MyOwner = Owner;

	InitButtonClickedEvents();
	
	LoadTab(UseableList, UseableTable);
	LoadTab(TurretList, TurretTable);
	LoadTab(TrapList, TrapTable);
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
		UCraftingItemWidget* CraftingItemWidget = CreateWidget<UCraftingItemWidget>(GetWorld(), CraftingItemWidgetClass);
		CraftingItemWidget->Init(*CraftingItem);
		
		ListWidget->AddChildToVerticalBox(CraftingItemWidget);
	}
}
