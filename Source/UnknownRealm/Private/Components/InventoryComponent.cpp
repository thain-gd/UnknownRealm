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

		for (const FInventoryItem& Item : Items)
		{
			UE_LOG(LogTemp, Warning, TEXT("You have %i %s"), Item.Count, *Item.ID.ToString());
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

bool UInventoryComponent::UseItems(const FName& CraftingItemID)
{
	UMPGameInstance* GameInstance = GetWorld()->GetGameInstance<UMPGameInstance>();
	FCraftingItem* CraftingItem = GameInstance->GetCraftingDataRow(CraftingItemID);
	auto Requirements = CraftingItem->Requirements; // verify if edit this one affects the row of udata after that

	// Decrease count to check if there are enough resources
	for (auto& Item: Items)
	{
		if (Requirements.Contains(Item.ID) && Requirements[Item.ID] > 0)
		{
			Requirements[Item.ID] = FMath::Min(0, Requirements[Item.ID] - Item.Count);
		}
	}

	// Reset requirement count
	for (auto& Requirement : Requirements)
	{
		// There is not enough resources
		if (Requirement.Value > 0)
		{
			return false;
		}
		
		Requirement.Value = CraftingItem->Requirements[Requirement.Key];
	}

	for (int i = 0; i < Items.Num(); ++i)
	{
		FInventoryItem& Item = Items[i];
		if (Requirements.Contains(Item.ID) && Requirements[Item.ID] > 0)
		{
			const int32 Available = Item.Count;
			Item.Count -= Requirements[Item.ID];
			Requirements[Item.ID] = FMath::Min(0, Requirements[Item.ID] - Available);
			if (Item.Count <= 0)
			{
				Items[i].Reset();
				--FreeSlots;
			}
		}
	}

	MulticastUpdateWidget(Items);
	return true;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, FreeSlots);
}
