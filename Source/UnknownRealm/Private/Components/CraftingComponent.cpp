// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CraftingComponent.h"

#include "Blueprint/UserWidget.h"
#include "Core/MPPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "UI/CraftingWidget.h"

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
	}
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCraftingComponent::ToggleWidget(ACharacter* Crafter) const
{
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(Crafter->Controller);
	
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

