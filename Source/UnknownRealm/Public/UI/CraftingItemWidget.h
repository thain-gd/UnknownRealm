// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CraftingComponent.h"

#include "CraftingItemWidget.generated.h"

class UButton;
class UItemWidget;
class UTextBlock;
class UImage;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UCraftingItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UCraftingComponent* InCraftingComp, const FName& InCraftingItemID, const FCraftingItem* CraftingItem);

	void UpdateRequirements(const TMap<FName, int32>& AvailableResources);
	
	FCraftingItem* GetCraftingItemSettings() const { return CraftingItemSettings; }

private:
	UFUNCTION()
	void StartCraftingItem();
	
	bool IsCraftable() const;

	void SetRequirementStatus(int32 RequirementIndex, bool bEnoughResources);


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CraftingBtn;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* RequirementList;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY()
	UCraftingComponent* CraftingComp;

	FName CraftingItemID;
	FCraftingItem* CraftingItemSettings;
};
