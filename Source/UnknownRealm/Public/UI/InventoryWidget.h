// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"

#include "InventoryWidget.generated.h"

class UItemWidget;
class UButton;
class UUniformGridPanel;
class UBorder;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class UNKNOWNREALM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(int32 Rows, int32 Columns);
	
	void Refresh(const TArray<FInventoryItem>& Items);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> ItemWidgetClass;

	TArray<UItemWidget*> ItemWidgets;
};
