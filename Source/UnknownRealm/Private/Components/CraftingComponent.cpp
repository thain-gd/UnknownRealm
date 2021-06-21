// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CraftingComponent.h"

#include "Blueprint/UserWidget.h"
#include "Core/MPPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "UI/CraftingWidget.h"
#include "Components/InventoryComponent.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetOwner());
	if (PlayerChar && PlayerChar->IsLocallyControlled())
	{
		CraftingWidget = CreateWidget<UCraftingWidget>(GetWorld(), CraftingWidgetClass);
		CraftingWidget->Init(this);
	}
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCraftingComponent::ToggleWidget() const
{
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(Cast<APlayerCharacter>(GetOwner())->Controller);
	
	if (CraftingWidget->IsInViewport())
	{
		if (PlayerController)
		{
			PlayerController->SetInputToGameOnly();
		}
		
		CraftingWidget->RemoveFromViewport();
	}
	else
	{
		if (PlayerController)
		{
			PlayerController->SetInputToGameAndUI();
		}
		
		CraftingWidget->AddToViewport();
	}
}

void UCraftingComponent::UpdateCraftingAvailabilities(const TArray<FInventoryItem>& ItemList)
{
	// Accumulate all resources that we have
	TMap<FName, int32> AvailableResources;
	for (const FInventoryItem& Item : ItemList)
	{
		if (AvailableResources.Contains(Item.ID))
		{
			AvailableResources[Item.ID] += Item.Count;
		}
		else
		{
			AvailableResources.Add(Item.ID, Item.Count);
		}
	}

	CraftingWidget->UpdateCraftableWidgets(AvailableResources);
}

