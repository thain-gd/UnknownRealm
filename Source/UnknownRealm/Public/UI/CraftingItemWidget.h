// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingItemWidget.generated.h"

class UItemWidget;
struct FCraftingItem;
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
	void Init(const FCraftingItem& CraftingItem);
	

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* RequirementList;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemWidget> ItemWidgetClass;
};
