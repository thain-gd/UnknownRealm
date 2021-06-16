// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "UI/InventoryWidget.h"

class AMPGameState;

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

	if (GetOwner()->HasAuthority())
	{
		Init();
	}
}

void UInventoryComponent::Init()
{
	FreeSlots = MaxRows * MaxColumns;

	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
}

void UInventoryComponent::AddItem(const FName& ItemID, const int32 Amount)
{
	FInventoryItem& CollectedItem = *ItemTable->FindRow<FInventoryItem>(ItemID, TEXT("UInventoryComponent::AddItem"), true);
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

		for (const FInventoryItem& Item : Items)
		{
			UE_LOG(LogTemp, Warning, TEXT("You have %i %s"), Item.Count, *Item.ID.ToString());
		}
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
	Items.Add(Item);
	--FreeSlots;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Items);
	DOREPLIFETIME(UInventoryComponent, FreeSlots);
}
