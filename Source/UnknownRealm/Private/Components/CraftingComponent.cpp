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

void UCraftingComponent::Init(UCameraComponent* FollowCam)
{
	CraftingCam = FollowCam;
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
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CraftingObject && GetOwner<APawn>()->IsLocallyControlled())
	{
		FVector StartPoint = CraftingCam->GetComponentLocation();
		FVector EndPoint = StartPoint + CraftingCam->GetForwardVector() * 600.0f;
		
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.AddIgnoredActor(CraftingObject);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_Visibility, TraceParams);

		if (Hit.bBlockingHit)
		{
			ServerUpdateCraftingObjectLocation(Hit.Location, true);
		}
		else
		{
			ServerUpdateCraftingObjectLocation(EndPoint, false);
		}
	}
}

void UCraftingComponent::ServerUpdateCraftingObjectLocation_Implementation(const FVector& NewLocation, bool bFoundPlacement)
{
	if (CraftingObject)
	{
		CraftingObject->SetActorLocation(NewLocation);
		if (bFoundPlacement)
		{
			const bool bBuildable = CraftingObject->CheckBuildStatus();
			CraftingObject->MulticastSetMaterials(bBuildable ? CanBuildMat : CanNotBuildMat);
		}
		else
		{
			CraftingObject->MulticastSetMaterials(CanNotBuildMat);
		}
	}
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

void UCraftingComponent::UpdateCraftingAvailabilities(const TArray<FInventoryItem>& ItemList) const
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

void UCraftingComponent::StartCrafting(FCraftingItem* CraftingItemSettings)
{
	ToggleWidget(); // Hide crafting menu

	if (*CraftingItemSettings->Class)
	{
		ServerSpawnCraftingObject(CraftingItemSettings->Class);
	}
}

void UCraftingComponent::ServerSpawnCraftingObject_Implementation(TSubclassOf<ACraftingObject> CraftingObjectClass)
{
	CraftingObject = GetWorld()->SpawnActor<ACraftingObject>(CraftingObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
	CraftingObject->MulticastInit(CanBuildMat);
}

void UCraftingComponent::ServerVerifyPlacement_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), CraftingObject->IsBuildable() ? TEXT("Buildable") : TEXT("Not Buildable"));
	if (!CraftingObject->IsBuildable())
		return;

	CraftingObject->MulticastConfirmPlacement();
	CraftingObject = nullptr;
}

void UCraftingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCraftingComponent, CraftingObject);
}

