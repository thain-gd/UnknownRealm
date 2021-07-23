// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CraftingComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
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

	void Tick(float DeltaSeconds) override;

	UFUNCTION(Reliable, Server)
	void ServerFinishCollecting(ACollectibleItem* CollectedItem);

	void UpdateCraftingMenu() const { CraftingComp->UpdateCraftingAvailabilities(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SetupWeapon();
	void SetupWeaponInputs();

	UFUNCTION()
	void SetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ResetAttackableEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ShowInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HideInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateCameraFOV(float DeltaTime);
	
	void MoveVertical(float AxisValue);
	void MoveHorizontal(float AxisValue);

	UFUNCTION(Reliable, Server)
	void ServerDoNormalAttack();

	UFUNCTION(Reliable, Server)
	void ServerDoHeavyAttack();

	UFUNCTION(Reliable, Server)
	void ServerGetWeapon();
	
	UFUNCTION(Reliable, Server)
	void ServerPutWeaponAway();

	UFUNCTION(Reliable, Server)
	void ServerOnAimingPressed();

	UFUNCTION(Reliable, Server)
	void ServerOnAimingReleased();

	UFUNCTION()
	void OnRepAimingStatusChanged();

	void OnAimingStart();
	void OnAimingEnd();

	UFUNCTION(Reliable, Server)
	void ServerOnChargingStart();

	UFUNCTION(Reliable, Server)
	void ServerOnChargingEnd();

	FVector CalculateTargetLocation() const;

	void Interact();

	void ToggleCraftMenu();

	void OnWheelAxisChanged(float AxisValue);

	UFUNCTION(BlueprintCallable)
	float GetChargeAmount() const;
	
	
private:
	static const FName InactiveWeaponSocketName;
	
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

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACollectibleItem* CollectibleItem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponID;

	UPROPERTY()
	TArray<AActor*> AttackableEnemies;
	
	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY(Replicated)
	AWeapon* Weapon;
	
	bool bInteractable;
	bool bInteracting;

	// Combat system
	UPROPERTY(EditAnywhere, Category = Combat)
	float AimingFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AimingInterpSpeed;

	UPROPERTY(Replicated)
	bool bUsingWeapon;
	
	UPROPERTY(ReplicatedUsing=OnRepAimingStatusChanged, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;
	float DefaultFOV;
	float AimingMovingSpeed;
	float DefaultMovingSpeed;
	uint32 AttackCount;
};
