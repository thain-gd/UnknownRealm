// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingObject.generated.h"

class UBoxComponent;
UCLASS()
class UNKNOWNREALM_API ACraftingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInit(UMaterialInstance* NewMaterial) const;

	void SetBuildability(bool bInIsBuidable, UMaterialInstance* NewMaterial);

	bool IsBuildable() const { return bIsBuildable; }

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMaterials(UMaterialInstance* NewMaterial) const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastConfirmPlacement() const;

	bool CheckBuildStatus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SaveDefaultMaterials();

	void SetMaterials(UMaterialInstance* NewMaterial) const;

	bool IsLandScape(AActor* Actor) const;

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BuildCollision;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* FoundationCollision;

	UPROPERTY()
	TArray<UMaterialInterface*> DefaultMaterials;

	UPROPERTY(Replicated)
	bool bIsBuildable;
};
