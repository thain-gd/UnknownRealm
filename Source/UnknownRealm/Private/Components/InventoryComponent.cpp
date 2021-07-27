// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Core/MPGameInstance.h"
#include "Core/MPPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "UI/InventoryWidget.h"

void FInventoryItem::Reset()
{
	ID = EMPTY_ID;
	Icon = nullptr;
	Count = 0;
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UInventoryComponent::Init()
{
	if (GetOwner()->HasAuthority())
	{
		FreeSlots = MaxRows * MaxColumns;
		Items.SetNum(FreeSlots);
	}

	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->Init(MaxRows, MaxColumns);
}

void UInventoryComponent::GetAvailableResources(TMap<FName, int32>& OutAvailableResources)
{
	for (const FInventoryItem& Item : Items)
	{
		if (OutAvailableResources.Contains(Item.ID))
		{
			OutAvailableResources[Item.ID] += Item.Count;
		}
		else
		{
			OutAvailableResources.Add(Item.ID, Item.Count);
		}
	}
}

void UInventoryComponent::ShowWidget() const
{
	InventoryWidget->AddToViewport();
}

void UInventoryComponent::HideWidget() const
{
	InventoryWidget->RemoveFromParent();
}

void UInventoryComponent::AddItem(const FName& ItemID, const int32 Amount)
{
	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
		return;
	
	FInventoryItem& CollectedItem = *GameInstance->GetInventoryItemData()->FindRow<FInventoryItem>(ItemID, TEXT("UInventoryComponent::AddItem"), true);
	if (CollectedItem.ID == ItemID)
	{
		CollectedItem.Count = Amount;
		if (CollectedItem.bStackable)
		{
			AddStackableItem(CollectedItem);
		}
		else if (FreeSlots > 0)
		{
			AddItemToNewSlot(CollectedItem);
		}

		MulticastUpdateWidget(Items);
	}
}

void UInventoryComponent::AddStackableItem(FInventoryItem StackableItem)
{
	for (FInventoryItem& Item : Items)
	{
		if (Item.ID == StackableItem.ID)
		{
			if (Item.Count + StackableItem.Count <= Item.MaxStackCount)
			{
				Item.Count += StackableItem.Count;
				return;
			}

			StackableItem.Count = Item.Count + StackableItem.Count - Item.MaxStackCount;
			Item.Count = Item.MaxStackCount;
		}
	}

	if (StackableItem.Count > 0 && FreeSlots > 0)
	{
		AddItemToNewSlot(StackableItem);
	}
}

void UInventoryComponent::AddItemToNewSlot(FInventoryItem& Item)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ID == EMPTY_ID)
		{
			Items[i] = Item;
			--FreeSlots;
			return;
		}
	}
}

bool UInventoryComponent::RemoveItems(TMap<FName, int32>& ToRemoveItems)
{
	if (!AreItemsAvailable(ToRemoveItems))
		return false;

	TArray<int32> ItemIndices;
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!ToRemoveItems.Contains(Items[i].ID))
			continue;

		ItemIndices.Add(i);
	}

	for (int ItemIndex : ItemIndices)
	{
		int32* RemainingAmountPtr = &ToRemoveItems[Items[ItemIndex].ID];
		if (*RemainingAmountPtr <= 0)
			continue;

		const int32 Available = Items[ItemIndex].Count;
		Items[ItemIndex].Count -= *RemainingAmountPtr;
		*RemainingAmountPtr -= Available;
		if (Items[ItemIndex].Count <= 0)
		{
			Items[ItemIndex].Reset();
			++FreeSlots;
		}
	}

	MulticastUpdateWidget(Items);
	return true;
}

bool UInventoryComponent::RemoveItem(const FName& ItemID, const int32 Amount)
{
	int32 RemainingAmount = Amount;
	TArray<int32> ItemIndices;
	for (int i = 0; i < Items.Num() && RemainingAmount > 0; ++i)
	{
		if (Items[i].ID != ItemID)
			continue;

		if (Items[i].Count >= RemainingAmount)
		{
			RemainingAmount = 0;
		}
		else
		{
			RemainingAmount -= Items[i].Count;
		}

		ItemIndices.Add(i);
	}

	if (RemainingAmount != 0)
		return false;

	RemainingAmount = Amount;
	for (int ItemIndex : ItemIndices)
	{
		const int32 Available = Items[ItemIndex].Count;
		Items[ItemIndex].Count -= RemainingAmount;
		RemainingAmount -= Available;
		if (Items[ItemIndex].Count <= 0)
		{
			Items[ItemIndex].Reset();
			++FreeSlots;
		}
	}

	MulticastUpdateWidget(Items);
	return true;
}

bool UInventoryComponent::AreItemsAvailable(const TMap<FName, int32>& ItemPairs)
{
	TMap<FName, int32> AvailableResources;
	GetAvailableResources(AvailableResources);

	for (auto& ItemPair : ItemPairs)
	{
		if (!AvailableResources.Contains(ItemPair.Key) || AvailableResources[ItemPair.Key] < ItemPair.Value)
		{
			return false;
		}
	}

	return true;
}

void UInventoryComponent::MulticastUpdateWidget_Implementation(const TArray<FInventoryItem>& ItemList)
{
	Items = ItemList;
	
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(PlayerController->GetPawn());
		if (PlayerChar)
		{
			PlayerChar->UpdateCraftingMenu();
		}
	}
	
	InventoryWidget->Refresh(ItemList);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, FreeSlots);
}
