// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CraftingComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UStaminaComponent;
class AWeapon;
struct FInventoryItem;
class UCraftingComponent;
class UInteractionWidget;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UHealthComponent;
class ACollectibleItem;

enum class EWeaponType : uint8;

UCLASS()
class UNKNOWNREALM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	void SetMovementForAiming() const;
	void ResetMovement() const;

	UFUNCTION(Reliable, Server)
	void ServerFinishCollecting(ACollectibleItem* CollectedItem);

	void UpdateCraftingMenu() const { CraftingComp->UpdateCraftingAvailabilities(); }

	UFUNCTION(BlueprintCallable)
	EWeaponType GetEquippedWeaponType() const;

	UCameraComponent* GetCameraComp() const { return CameraComp; }

	UAnimInstance* GetAnimInstance() const;
	
	float GetStaminaPercent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void UpdateCameraFOV(float DeltaSeconds);

	UFUNCTION(Server, Unreliable)
	void ServerUpdateAimingRotation(const FRotator& NewRotation);

	UFUNCTION(Server, Reliable)
	void ServerSetupWeapon(AActor* WeaponOwner);

	UFUNCTION()
	void ShowInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HideInteractingUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void MoveVertical(float AxisValue);
	void MoveHorizontal(float AxisValue);

	void StartSprinting();
	void StopSprinting();

	void OnSpaceActionsPressed();

	UFUNCTION(Server, Reliable)
	void SR_DodgeRoll();

	UFUNCTION(Server, Reliable)
	void SR_DoSideStep(bool bIsLeft);

	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayAnimMontage(UAnimMontage* MontageToPlay);
	
	UFUNCTION(Server, Reliable)
	void SR_PutWeaponAway();

	void Interact();

	void ToggleCraftMenu();

	void OnWheelAxisChanged(float AxisValue);

	UFUNCTION(BlueprintCallable)
	float GetChargeAmount() const;
	
	
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
	UStaminaComponent* StaminaComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCraftingComponent* CraftingComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACollectibleItem* CollectibleItem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponID;

	UPROPERTY(EditAnywhere, Category = "Animation | Dodge")
	UAnimMontage* DodgeRollMontage;

	UPROPERTY(EditAnywhere, Category = "Animation | Dodge")
	float DodgeStaminaPercent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Side Step")
	UAnimMontage* LeftSideStepMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Side Step")
	UAnimMontage* RightSideStepMontage;
	
	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AWeapon* Weapon;
	
	bool bInteractable;
	bool bInteracting;

	// Combat system
	UPROPERTY(EditAnywhere, Category = Combat)
	float AimingFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AimingInterpSpeed;

	float DefaultFOV;
	float AimingMovingSpeed;
	float DefaultMovingSpeed;
};
