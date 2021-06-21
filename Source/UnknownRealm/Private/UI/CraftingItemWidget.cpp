// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingItemWidget.h"

#include "Components/Button.h"
#include "Components/CraftingComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/MPGameInstance.h"
#include "UI/ItemWidget.h"
#include "UI/CraftingWidget.h"

void UCraftingItemWidget::Init(UCraftingWidget* Owner, const FCraftingItem* CraftingItem)
{
	MyOwner = Owner;
	CraftingItemSettings = const_cast<FCraftingItem*>(CraftingItem);
	
	Icon->SetBrushFromTexture(CraftingItem->Icon);
	Name->SetText(CraftingItem->Name);

	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	for (const auto& Requirement : CraftingItem->Requirements)
	{
		FInventoryItem* Item = GameInstance->GetInventoryItemData()->FindRow<FInventoryItem>(Requirement.Key, TEXT("UCraftingItemWidget::Init"), true);
		if (Item)
		{
			UItemWidget* RequirementWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
			RequirementWidget->Init(Item->Icon, Requirement.Value);
			RequirementWidget->SetCountStatus(false);

			RequirementList->AddChildToHorizontalBox(RequirementWidget);
		}
	}

	CraftingBtn->OnClicked.AddDynamic(this, &UCraftingItemWidget::StartCraftingItem);
}

void UCraftingItemWidget::SetRequirementStatus(int32 RequirementIndex, bool bEnoughResources)
{
	UItemWidget* RequirementWidget = Cast<UItemWidget>(RequirementList->GetChildAt(RequirementIndex));
	if (RequirementWidget)
	{
		RequirementWidget->SetCountStatus(bEnoughResources);
	}
}

void UCraftingItemWidget::StartCraftingItem()
{
	if (!IsCraftable())
		return;
	
	MyOwner->StartCrafting(CraftingItemSettings);
}

bool UCraftingItemWidget::IsCraftable() const
{
	TArray<UWidget*> RequirementWidgets = RequirementList->GetAllChildren();
	for (int i = 0; i < RequirementWidgets.Num(); ++i)
	{
		UItemWidget* RequirementWidget = Cast<UItemWidget>(RequirementWidgets[i]);
		if (!RequirementWidget->AreResourcesEnough())
			return false;
	}

	return true;
}
