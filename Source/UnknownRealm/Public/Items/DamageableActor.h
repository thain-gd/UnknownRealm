// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageableActor.generated.h"

UCLASS()
class UNKNOWNREALM_API ADamageableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageableActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHealthChanged();
	

protected:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComp;

};
