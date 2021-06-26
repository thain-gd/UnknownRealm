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

	void Init(UCameraComponent* FollowCam);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleWidget() const;

	void UpdateCraftingAvailabilities() const;

	void StartCrafting(const FName& CraftingItemID, FCraftingItem* CraftingItemSettings);

	UFUNCTION(Server, Reliable)
	void ServerVerifyPlacement();
	
	bool IsCrafting() const { return CraftingObject != nullptr; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION(Server, Reliable)
	void ServerSpawnCraftingObject(const FName& CraftingItemID, TSubclassOf<ACraftingObject> CraftingObjectClass);

	UFUNCTION(Server, Reliable)
	void ServerUpdateCraftingObjectLocation(const FVector& NewLocation, bool bFoundPlacement);


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
