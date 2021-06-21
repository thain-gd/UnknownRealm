// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CraftingComponent.h"

#include "CraftingItemWidget.generated.h"

class UCraftingWidget;
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
	void Init(UCraftingWidget* Owner, const FCraftingItem* CraftingItem);

	void SetRequirementStatus(int32 RequirementIndex, bool bEnoughResources);
	
	FCraftingItem* GetCraftingItemSettings() const { return CraftingItemSettings; }

private:
	UFUNCTION()
	void StartCraftingItem();
	
	bool IsCraftable() const;
	

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
	UCraftingWidget* MyOwner;

	FCraftingItem* CraftingItemSettings;
};
