// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingItemWidget.h"

#include "Components/CraftingComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/ItemWidget.h"

void UCraftingItemWidget::Init(const FCraftingItem& CraftingItem)
{
	Icon->SetBrushFromTexture(CraftingItem.Icon);
	Name->SetText(CraftingItem.Name);

	for (const auto& Requirement : CraftingItem.Requirements)
	{
		UItemWidget* RequirementWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
		//RequirementWidget->Init(Re)
		RequirementList->AddChildToHorizontalBox(RequirementWidget);
	}
}
