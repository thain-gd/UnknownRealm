// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UButton;
class UUniformGridPanel;
class UBorder;
class UHorizontalBox;
struct FInventoryItem;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Refresh(const TArray<FInventoryItem>& Items);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void CloseWidget();

	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* InventoryBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CloseBtn;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* Categories;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* ItemGrid;
};
