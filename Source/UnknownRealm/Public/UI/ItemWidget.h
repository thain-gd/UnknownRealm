// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "ItemWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetails(const FInventoryItem& Item);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountNumber;
};
