// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CraftingComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInventoryItem;
class UCraftingComponent;
class UInteractionWidget;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UHealthComponent;
class ACollectibleItem;

UCLASS()
class UNKNOWNREALM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Reliable, Server)
	void ServerFinishCollecting(ACollectibleItem* CollectedItem);

	void UpdateCraftingMenu() const { CraftingComp->UpdateCraftingAvailabilities(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ResetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ShowInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HideInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void MoveVertical(float AxisValue);
	void MoveHorizontal(float AxisValue);

	void Attack();

	void Interact();

	UFUNCTION(Reliable, Server)
	void ServerAttack();

	void ToggleCraftMenu();
	
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* AttackBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCraftingComponent* CraftingComp;

	UPROPERTY()
	TArray<AActor*> AttackableEnemies;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACollectibleItem* CollectibleItem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractionWidgetClass;
	
	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	
	bool bInteractable;
	bool bInteracting;
};
