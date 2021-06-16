// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "UI/InventoryWidget.h"

#include "InventoryComponent.generated.h"

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

	UPROPERTY(EditDefaultsOnly)
	FName ID;

	UPROPERTY(EditDefaultsOnly)
	int32 Count;

	UPROPERTY(EditDefaultsOnly)
	bool bStackable;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackCount;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

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

	void AddItem(const FName& ItemID, const int32 Amount);

	int32 GetFreeSlots() const { return FreeSlots; }

	void ShowWidget() const { InventoryWidget->AddToViewport(); }
	void HideWidget() const { InventoryWidget->RemoveFromParent(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void Init();

	void AddStackableItem(FInventoryItem StackableItem);
	void AddItemToNewSlot(FInventoryItem& Item);

	UFUNCTION(Reliable, NetMulticast)
	void MulticastUpdateWidget();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UDataTable* ItemTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxRows;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxColumns;

	UPROPERTY(Replicated)
	TArray<FInventoryItem> Items;

	UPROPERTY(Replicated)
	int32 FreeSlots;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;
};
