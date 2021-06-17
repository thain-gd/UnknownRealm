// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemWidget::UpdateDetails(const FInventoryItem& Item)
{
	Icon->SetVisibility(ESlateVisibility::Visible);
	Icon->SetBrushFromTexture(Item.Icon);
	CountNumber->SetText(FText::AsNumber(Item.Count));
}
