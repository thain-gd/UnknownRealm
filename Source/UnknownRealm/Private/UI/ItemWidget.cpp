// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemWidget::Init(UTexture2D* ItemIcon, int32 Count)
{
	Icon->SetVisibility(ESlateVisibility::Visible);
	Icon->SetBrushFromTexture(ItemIcon);

	// Items that are not stackable, hide the count number
	if (Count == 0)
	{
		CountNumber->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CountNumber->SetText(FText::AsNumber(Count));
		CountNumber->SetVisibility(ESlateVisibility::Visible);
	}
}
