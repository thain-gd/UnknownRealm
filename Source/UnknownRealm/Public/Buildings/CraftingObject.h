// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingObject.generated.h"

UCLASS()
class UNKNOWNREALM_API ACraftingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(UMaterialInstance* NewMaterial) const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMaterials(UMaterialInstance* NewMaterial) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;
};
