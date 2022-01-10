// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ServerAnimNotify.generated.h"

class UWorld;

UCLASS()
class UNKNOWNREALM_API UServerAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UServerAnimNotify();
	
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UWorld* GetWorld() const override { return WorldContext; }

	
private:
	UPROPERTY()
	UWorld* WorldContext;
};
