// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "Equipment.generated.h"

UENUM(BlueprintType)
enum class EEquipmentCategory : uint8
{
	Weapon,
	Helm,
	Chest,
	Gloves,
	Leggings,
};

USTRUCT(BlueprintType)
struct FEquipmentInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> Class;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	EEquipmentCategory Category;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, int32> Requirements;
};

UCLASS(Abstract)
class UNKNOWNREALM_API AEquipment : public AActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	AEquipment();
	
	virtual void Init(FEquipmentInfo* InEquipInfo);
	
	FName GetAttachPoint() const { return ActiveSocketName; };

private:
	UFUNCTION()
	void OR_SetStaticMesh();

	UFUNCTION()
	void OR_SetSkeletalMesh();
	

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(ReplicatedUsing = OR_SetStaticMesh)
	UStaticMesh* StaticMesh;

	UPROPERTY(ReplicatedUsing = OR_SetSkeletalMesh)
	USkeletalMesh* SkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly)
	FName ActiveSocketName;
	
	FEquipmentInfo* EquipInfo;
};
