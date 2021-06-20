// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingItemWidget.h"

#include "Components/CraftingComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/MPGameInstance.h"
#include "UI/ItemWidget.h"

void UCraftingItemWidget::Init(const FCraftingItem& CraftingItem)
{
	Icon->SetBrushFromTexture(CraftingItem.Icon);
	Name->SetText(CraftingItem.Name);

	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	if (!GameInstance)
		return;

	for (const auto& Requirement : CraftingItem.Requirements)
	{
		FInventoryItem* Item = GameInstance->GetInventoryItemData()->FindRow<FInventoryItem>(Requirement.Key, TEXT("UCraftingItemWidget::Init"), true);
		if (Item)
		{
			UItemWidget* RequirementWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
			RequirementWidget->Init(Item->Icon, Requirement.Value);

			RequirementList->AddChildToHorizontalBox(RequirementWidget);
		}
	}
}
