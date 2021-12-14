// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Containers/Map.h"

#include "CraftingComponent.generated.h"


class UCameraComponent;
struct FInventoryItem;
class ACraftingObject;

USTRUCT(BlueprintType)
struct FCraftingItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACraftingObject> Class;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	int32 AmountPerCraftTime = 1;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, int32> Requirements;
};

class UCraftingWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	void Init(UCameraComponent* InCraftingCam);

	// Called every frame
	void TickComponent(float InDeltaTime, ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) override;

	void ToggleCraftingWidget();

	void UpdateCraftingAvailabilities() const;

	void StartCrafting(const FName& InCraftingItemID, FCraftingItem* InCraftingItemSettings);

	void VerifyPlacement();
	
	bool IsCrafting() const { return CraftingObject != nullptr; }

	UFUNCTION(Server, Reliable)
	void SR_RotateCraftingObject(float InAxisValue);

	UFUNCTION(Server, Reliable)
	void SR_CraftUseables(const FName& InUseableID, const int32 InDefaultAmount, const int32 InCraftTime) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void GetCraftingRequirements(const FName& UseableID, TMap<FName, int32>& OutRequirements, const int32 CraftTime = 1) const;
	
	UFUNCTION(Server, Reliable)
	void SR_SpawnCraftingObject(const FName& InCraftingItemID, TSubclassOf<ACraftingObject> InCraftingObjectClass);

	UFUNCTION(Server, Reliable)
	void SR_UpdateCraftingObjectLocation(const FVector& InNewLocation, bool bInFoundPlacement);

	void CancelCrafting();

	UFUNCTION(Server, Reliable)
	void SR_PlaceCraftingObject();

	void ShowCraftingWidget() const;
	void HideCraftingWidget() const;


private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CraftingWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* CanBuildMat;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* CanNotBuildMat;

	UPROPERTY()
	UCraftingWidget* CraftingWidget;

	UPROPERTY()
	UCameraComponent* CraftingCam;
	
	UPROPERTY(Replicated)
	ACraftingObject* CraftingObject;

	FName SelectedCraftingItemID;
};
