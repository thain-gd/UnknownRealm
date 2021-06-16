// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"

#include "Components/Button.h"
#include "Core/MPPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
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
	
}
