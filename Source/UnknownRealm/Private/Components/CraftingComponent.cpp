// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CraftingComponent.h"

#include "Blueprint/UserWidget.h"
#include "Core/MPPlayerController.h"
#include "Player/PlayerCharacter.h"
#include "UI/CraftingWidget.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Buildings/CraftingObject.h"
#include "Camera/CameraComponent.h"
#include "Core/MPGameInstance.h"
#include "Core/MPGameState.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCraftingComponent::Init(UCameraComponent* InCraftingCam)
{
	CraftingCam = InCraftingCam;
}

// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* PlayerChar = GetOwner<APlayerCharacter>();
	if (PlayerChar && PlayerChar->IsLocallyControlled())
	{
		CraftingWidget = CreateWidget<UCraftingWidget>(GetWorld(), CraftingWidgetClass);
		CraftingWidget->Init(this);
	}
}

// Called every frame
void UCraftingComponent::TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction)
{
	Super::TickComponent(InDeltaTime, InTickType, InThisTickFunction);

	if (CraftingObject && GetOwner<APawn>()->IsLocallyControlled())
	{
		FVector StartPoint = CraftingCam->GetComponentLocation();
		FVector EndPoint = StartPoint + CraftingCam->GetForwardVector() * 800.0f;
		
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.AddIgnoredActor(CraftingObject);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_Visibility, TraceParams);

		if (Hit.bBlockingHit)
		{
			SR_UpdateCraftingObjectLocation(Hit.Location, true);
		}
		else
		{
			SR_UpdateCraftingObjectLocation(EndPoint, false);
		}
	}
}

void UCraftingComponent::SR_CraftUseables_Implementation(const FName& InUseableID, const int32 InDefaultAmount, const int32 InCraftTime) const
{
	TMap<FName, int32> Requirements;
	GetCraftingRequirements(InUseableID, Requirements, InCraftTime);
	
	UInventoryComponent* Inventory = GetWorld()->GetGameState<AMPGameState>()->GetInventory();
	// TODO: Check if slots are enough
	if (Inventory->RemoveItems(Requirements))
	{
		Inventory->AddItem(InUseableID, InDefaultAmount * InCraftTime);
	}
}

void UCraftingComponent::GetCraftingRequirements(const FName& UseableID, TMap<FName, int32>& OutRequirements, const int32 CraftTime) const
{
	UMPGameInstance* GameInstance = GetWorld()->GetGameInstance<UMPGameInstance>();
	FCraftingItem* CraftingItem = GameInstance->GetCraftingDataRow(UseableID);
	OutRequirements = CraftingItem->Requirements; // verify if edit this one affects the row of udata after that

	// Update requirements by number of crafting time
	if (CraftTime > 1)
	{
		for (auto& Requirement : OutRequirements)
		{
			Requirement.Value *= CraftTime;
		}
	}
}

void UCraftingComponent::SR_UpdateCraftingObjectLocation_Implementation(const FVector& InNewLocation, bool bInFoundPlacement)
{
	if (CraftingObject)
	{
		CraftingObject->SetActorLocation(InNewLocation);
		if (bInFoundPlacement)
		{
			const bool bBuildable = CraftingObject->CheckBuildStatus();
			CraftingObject->MC_SetMaterials(bBuildable ? CanBuildMat : CanNotBuildMat);
		}
		else
		{
			CraftingObject->MC_SetMaterials(CanNotBuildMat);
		}
	}
}

void UCraftingComponent::ToggleCraftingWidget()
{
	if (CraftingWidget->IsInViewport())
	{
		if (CraftingObject)
		{
			CancelCrafting();
			return;
		}
		
		HideCraftingWidget();
	}
	else
	{
		ShowCraftingWidget();
	}
}

void UCraftingComponent::HideCraftingWidget() const
{
	Cast<AMPPlayerController>(GetOwner<APlayerCharacter>()->Controller)->SetInputToGameOnly();
	CraftingWidget->Hide();
}

void UCraftingComponent::CancelCrafting()
{
	Cast<AMPPlayerController>(GetOwner<APlayerCharacter>()->Controller)->SR_DestroyActor(CraftingObject);
	CraftingObject = nullptr;
	Cast<AMPPlayerController>(Cast<APlayerCharacter>(GetOwner())->Controller)->SetInputToGameAndUI();
	CraftingWidget->HideCraftingGuidelines();
}

void UCraftingComponent::ShowCraftingWidget() const
{
	Cast<AMPPlayerController>(Cast<APlayerCharacter>(GetOwner())->Controller)->SetInputToGameAndUI();
	CraftingWidget->Show();
}

void UCraftingComponent::UpdateCraftingAvailabilities() const
{
	CraftingWidget->UpdateCraftableWidgets();
}

void UCraftingComponent::StartCrafting(const FName& InCraftingItemID, FCraftingItem* InCraftingItemSettings)
{
	Cast<AMPPlayerController>(GetOwner<APlayerCharacter>()->Controller)->SetInputToGameOnly();
	CraftingWidget->ShowCraftingGuidelines();
	SR_SpawnCraftingObject(InCraftingItemID, InCraftingItemSettings->Class);
}

void UCraftingComponent::SR_SpawnCraftingObject_Implementation(const FName& InCraftingItemID, TSubclassOf<ACraftingObject> InCraftingObjectClass)
{
	SelectedCraftingItemID = InCraftingItemID;
	CraftingObject = GetWorld()->SpawnActor<ACraftingObject>(InCraftingObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
	CraftingObject->Init(CanBuildMat);
}

void UCraftingComponent::VerifyPlacement()
{
	if (!CraftingObject->IsBuildable())
		return;

	HideCraftingWidget();
	SR_PlaceCraftingObject();
}

void UCraftingComponent::SR_PlaceCraftingObject_Implementation()
{
	TMap<FName, int32> Requirements;
	GetCraftingRequirements(SelectedCraftingItemID, Requirements);
	
	if (GetWorld()->GetGameState<AMPGameState>()->GetInventory()->RemoveItems(Requirements))
	{
		CraftingObject->MC_ConfirmPlacement();
	}
	else
	{
		// TODO: Notify player that there are not enough resources
		GetWorld()->DestroyActor(CraftingObject);
	}

	CraftingObject = nullptr;
}

void UCraftingComponent::SR_RotateCraftingObject_Implementation(float InAxisValue)
{
	CraftingObject->AddActorLocalRotation(FRotator(0.0f, InAxisValue * 15.0f, 0.0f));
}

void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCraftingComponent, CraftingObject);
}

