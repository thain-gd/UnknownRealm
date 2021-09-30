// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CraftingItemWidget.h"

#include "Components/Button.h"
#include "Components/CraftingComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/MPGameInstance.h"
#include "Core/MPGameState.h"
#include "Misc/DefaultValueHelper.h"
#include "UI/ItemWidget.h"
#include "Buildings/CraftingObject.h"

#define MIN_CRAFT_TIME 1
#define MAX_CRAFT_TIME 99

void UCraftingItemWidget::Init(bool bInIsUseable, UCraftingComponent* InCraftingComp, const FName& InCraftingItemID, const FCraftingItem* CraftingItem)
{
	bIsUseable = bInIsUseable;
	CraftingComp = InCraftingComp;
	CraftingItemID = InCraftingItemID;
	CraftingItemSettings = const_cast<FCraftingItem*>(CraftingItem);
	
	Icon->SetBrushFromTexture(CraftingItem->Icon);
	Name->SetText(CraftingItem->Name);

	UMPGameInstance* GameInstance = Cast<UMPGameInstance>(GetGameInstance());
	for (const auto& Requirement : CraftingItem->Requirements)
	{
		FInventoryItem* Item = GameInstance->GetInventoryItemData()->FindRow<FInventoryItem>(Requirement.Key, TEXT("UCraftingItemWidget::Init"), true);
		if (Item)
		{
			UItemWidget* RequirementWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
			RequirementWidget->Init(Item->Icon, Requirement.Value);
			RequirementWidget->SetCountStatus(false);

			RequirementList->AddChildToHorizontalBox(RequirementWidget);
		}
	}

	CraftingBtn->OnClicked.AddDynamic(this, &UCraftingItemWidget::StartCraftingItem);

	if (bIsUseable)
	{
		UseableCraftingBox->SetVisibility(ESlateVisibility::Visible);
		DefaultAmount->SetText(FText::AsNumber(CraftingItemSettings->AmountPerCraftTime));
		
		CraftTimeTextBox->OnTextCommitted.AddDynamic(this, &UCraftingItemWidget::OnTextCommitted);
		IncreaseCraftTimeBtn->OnClicked.AddDynamic(this, &UCraftingItemWidget::IncreaseCraftTime);
		DecreaseCraftTimeBtn->OnClicked.AddDynamic(this, &UCraftingItemWidget::DecreaseCraftTime);
	}
	else
	{
		UseableCraftingBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCraftingItemWidget::Reset() const
{
	CraftTimeTextBox->SetText(FText::AsNumber(1));

	TArray<UWidget*> RequirementWidgets = RequirementList->GetAllChildren();
	uint32 i = 0;
	for (auto& Requirement : CraftingItemSettings->Requirements)
	{
		Cast<UItemWidget>(RequirementWidgets[i])->SetCountNumberText(Requirement.Value);

		++i;
	}
}

void UCraftingItemWidget::VerifyRequirements(const TMap<FName, int32>& AvailableResources, TMap<FName, int32>* RequirementsPtr)
{
	const TMap<FName, int32>& Requirements = RequirementsPtr ? *RequirementsPtr : CraftingItemSettings->Requirements;
	uint32 RequirementIndex = 0;
	for (auto& Requirement : Requirements)
	{
		if (!AvailableResources.Contains(Requirement.Key) || AvailableResources[Requirement.Key] < Requirement.Value)
		{
			SetRequirementStatus(RequirementIndex, false);
		}
		else
		{
			SetRequirementStatus(RequirementIndex, true);
		}

		++RequirementIndex;
	}
}

void UCraftingItemWidget::SetRequirementStatus(int32 RequirementIndex, bool bEnoughResources)
{
	UItemWidget* RequirementWidget = Cast<UItemWidget>(RequirementList->GetChildAt(RequirementIndex));
	if (RequirementWidget)
	{
		RequirementWidget->SetCountStatus(bEnoughResources);
	}
}

void UCraftingItemWidget::StartCraftingItem()
{
	if (!IsCraftable())
		return;

	// Non-useables (traps/turrets)
	int32 CraftTime = FCString::Atoi(*CraftTimeTextBox->GetText().ToString());
	if (*CraftingItemSettings->Class)
	{
		CraftingComp->StartCrafting(CraftingItemID, CraftingItemSettings);
	}
	// Useables
	else 
	{
		CraftingComp->ToggleCraftingWidget();
		CraftingComp->SR_CraftUseables(CraftingItemID, FCString::Atoi(*DefaultAmount->GetText().ToString()), CraftTime);
	}
}

bool UCraftingItemWidget::IsCraftable() const
{
	TArray<UWidget*> RequirementWidgets = RequirementList->GetAllChildren();
	for (int i = 0; i < RequirementWidgets.Num(); ++i)
	{
		UItemWidget* RequirementWidget = Cast<UItemWidget>(RequirementWidgets[i]);
		if (!RequirementWidget->AreResourcesEnough())
			return false;
	}

	return true;
}

void UCraftingItemWidget::OnTextCommitted(const FText& InCraftTimeText, ETextCommit::Type CommitMethod)
{
	int32 CraftTime;
	if (!FDefaultValueHelper::ParseInt(InCraftTimeText.ToString(), CraftTime))
	{
		CraftTime = 1;
		CraftTimeTextBox->SetText(FText::AsNumber(CraftTime));
	}
	
	UpdateRequirements(CraftTime);
}

void UCraftingItemWidget::IncreaseCraftTime()
{
	const int32 CurrCraftTime = FCString::Atoi(*CraftTimeTextBox->GetText().ToString());
	const int32 NewCraftTime = FMath::Min(MAX_CRAFT_TIME, CurrCraftTime + 1);
	if (CurrCraftTime == NewCraftTime)
		return;
	
	CraftTimeTextBox->SetText(FText::AsNumber(NewCraftTime));
	UpdateRequirements(NewCraftTime);
}

void UCraftingItemWidget::DecreaseCraftTime()
{
	const int32 CurrCraftTime = FCString::Atoi(*CraftTimeTextBox->GetText().ToString());
	const int32 NewCraftTime = FMath::Max(MIN_CRAFT_TIME, CurrCraftTime - 1);
	if (CurrCraftTime == NewCraftTime)
		return;
	
	CraftTimeTextBox->SetText(FText::AsNumber(NewCraftTime));
	UpdateRequirements(NewCraftTime);
}

void UCraftingItemWidget::UpdateRequirements(int32 CraftTime)
{
	// Update the count of each requirement in the UI
	int32 i = 0;
	TArray<UWidget*> RequirementWidgets = RequirementList->GetAllChildren();
	auto Requirements = CraftingItemSettings->Requirements;
	for (auto& Requirement : Requirements)
	{
		Requirement.Value *= CraftTime;
		Cast<UItemWidget>(RequirementWidgets[i])->SetCountNumberText(Requirement.Value);

		++i;
	}

	// Check if there are enough resources
	TMap<FName, int32> AvailableResources;
	GetWorld()->GetGameState<AMPGameState>()->GetInventory()->GetAvailableResources(AvailableResources);
	VerifyRequirements(AvailableResources, &Requirements);
}
