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
	void Init(UTexture2D* ItemIcon, int32 Count);
	
	void SetCountStatus(bool bEnoughCount);
	void SetCountNumberText(int32 Count);

	bool AreResourcesEnough() const { return bResourcesEnough; };


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CountNumber;

	bool bResourcesEnough;
};
