// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleItem.generated.h"

class UTextRenderComponent;
class USphereComponent;

UCLASS()
class UNKNOWNREALM_API ACollectibleItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleItem();

	void OnFinishedCollecting();

	float GetCollectionTime() const { return CollectionTime; }

	const FString& GetInteractString() const { return InteractString; }

	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere;

	UPROPERTY(EditDefaultsOnly)
	FString InteractString;

	UPROPERTY(EditDefaultsOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CollectionTime;

	UPROPERTY(EditDefaultsOnly)
	int32 MinCollectibleAmount;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxCollectibleAmount;
};
