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

	UFUNCTION(Server, Reliable)
	void SR_FinishCollecting(ACollectibleItem* CollectedItem);

	void UpdateCraftingMenu() const { CraftingComp->UpdateCraftingAvailabilities(); }

	void ShowDamageDealt(const float InDealtDamage) const;

	UFUNCTION(Client, Reliable)
	void CL_ShowDamageDealt(const float InDealtDamage) const;

	UFUNCTION(BlueprintCallable)
	EWeaponType GetEquippedWeaponType() const;

	UCameraComponent* GetCameraComp() const { return CameraComp; }

	UAnimInstance* GetAnimInstance() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
	float GetStaminaPercent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHealthChanged();
	
	void UpdateCameraFOV(float DeltaSeconds);

	UFUNCTION(Server, Unreliable)
	void SR_UpdateAimingRotation(const FRotator& NewRotation);

	UFUNCTION(Server, Reliable)
	void SR_SetupWeapon(AActor* WeaponOwner);

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
	void ActivateInvincibility();

	UFUNCTION(BlueprintCallable)
	void DeactivateInvincibility();

	UFUNCTION(BlueprintCallable)
	bool GetInvincibility() const;

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

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanSideStep;

	float DefaultFOV;
	float AimingMovingSpeed;
	float DefaultMovingSpeed;
};
