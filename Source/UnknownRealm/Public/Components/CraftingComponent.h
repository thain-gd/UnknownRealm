// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingComponent.generated.h"


class UCraftingWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNKNOWNREALM_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleWidget(ACharacter* Crafter) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CraftingWidgetClass;

	UPROPERTY()
	UCraftingWidget* CraftingWidget;
};
