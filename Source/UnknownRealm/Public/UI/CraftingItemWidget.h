// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CraftingComponent.h"

#include "CraftingItemWidget.generated.h"

class UVerticalBox;
class UEditableTextBox;
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
	void Init(bool bInIsUseable, UCraftingComponent* InCraftingComp, const FName& InCraftingItemID, const FCraftingItem* CraftingItem);

	void VerifyRequirements(const TMap<FName, int32>& AvailableResources, TMap<FName, int32>* RequirementsPtr = nullptr);
	
	FCraftingItem* GetCraftingItemSettings() const { return CraftingItemSettings; }

private:
	UFUNCTION()
	void StartCraftingItem();

	UFUNCTION()
	void OnTextCommitted(const FText& InCraftTimeText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void IncreaseCraftTime();

	UFUNCTION()
	void DecreaseCraftTime();
	
	void UpdateRequirements(int32 CraftTime);
	
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
	UVerticalBox* UseableCraftingBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* CraftTimeTextBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* IncreaseCraftTimeBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DecreaseCraftTimeBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DefaultAmount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* RequirementList;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY()
	UCraftingComponent* CraftingComp;

	FName CraftingItemID;
	FCraftingItem* CraftingItemSettings;
	bool bIsUseable;
};
