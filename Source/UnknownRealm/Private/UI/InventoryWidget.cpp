// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Core/MPPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "UI/ItemWidget.h"

void UInventoryWidget::Init(int32 Rows, int32 Columns)
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InventoryBorder->Slot);
	if (CanvasSlot)
	{
		CanvasSlot->SetAutoSize(true);
	}

	for (int r = 0; r < Rows; r++)
	{
		for (int c = 0; c < Columns; c++)
		{
			UItemWidget* ItemWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
			ItemGrid->AddChildToUniformGrid(ItemWidget, r, c);
			ItemWidgets.Add(ItemWidget);
		}
	}

	if (CloseBtn)
	{
		CloseBtn->OnClicked.AddDynamic(this, &UInventoryWidget::CloseWidget);
	}
}

void UInventoryWidget::CloseWidget()
{
	RemoveFromParent();
	
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->SetInputToGameOnly();
		PlayerController->bInventoryOn = false;
	}
}

void UInventoryWidget::Refresh(const TArray<FInventoryItem>& Items)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		ItemWidgets[i]->Init(Items[i].Icon, Items[i].Count);
	}
}
