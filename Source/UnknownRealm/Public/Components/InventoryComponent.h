// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"

#include "InventoryComponent.generated.h"

#define EMPTY_ID TEXT("None")

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Material,
	Consumable
};

UENUM(BlueprintType)
enum class EItemAction : uint8
{
	Use,
	Craft,
	Discard,
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

	// Make it as an invalid item (for empty slots)
	void Reset();

	UPROPERTY(EditDefaultsOnly)
	FName ID = EMPTY_ID;

	UPROPERTY(EditDefaultsOnly)
	int32 Count = 0;

	UPROPERTY(EditDefaultsOnly)
	bool bStackable;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackCount = 0;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	EItemCategory Category;

	UPROPERTY(EditDefaultsOnly)
	TArray<EItemAction> ContextMenues;
};

class UInventoryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	void AddItem(const FName& ItemID, const int32 Amount = 1);
	bool RemoveItems(TMap<FName, int32>& ToRemoveItems);
	bool RemoveItem(const FName& ItemID, const int32 Amount = 1);

	bool AreItemsAvailable(const TMap<FName, int32>& ItemPairs);

	// Accumulate and return all resources that we have
	void GetAvailableResources(TMap<FName, int32>& OutAvailableResources);

	int32 GetFreeSlots() const { return FreeSlots; }

	void ShowWidget() const;
	void HideWidget() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void Init();

	void AddStackableItem(FInventoryItem StackableItem);
	void AddItemToNewSlot(FInventoryItem& Item);

	UFUNCTION(Reliable, NetMulticast)
	void MulticastUpdateWidget(const TArray<FInventoryItem>& ItemList);
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxRows;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxColumns;

	UPROPERTY(Replicated)
	int32 FreeSlots;

	UPROPERTY()
	TArray<FInventoryItem> Items;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;
};
