// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"

#include "CraftingWidget.generated.h"

struct FCraftingItem;
class UVerticalBox;
class UWidgetSwitcher;
class UCraftingComponent;
class UCraftingItemWidget;
class UButton;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UCraftingComponent* InCraftingComp);

	void UpdateCraftableWidgets();

private:
	void InitButtonClickedEvents();

	UFUNCTION()
	void SwitchToUseableTab();
	
	UFUNCTION()
	void SwitchToTurretTab();

	UFUNCTION()
	void SwitchToTrapTab();
	
	void SwitchToTab(int32 TabIndex) const;
	void LoadTab(UVerticalBox* ListWidget, UDataTable* CraftingList, bool bIsUseable = false);
	
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* TabSwitcher;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* UseablesBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TurretsBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TrapsBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* UseableList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* TurretList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* TrapList;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCraftingItemWidget> CraftingItemWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCraftingComponent* CraftingComp;

	UPROPERTY()
	TArray<UCraftingItemWidget*> CraftingItemWidgets;
};
