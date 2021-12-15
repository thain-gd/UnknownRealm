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
	virtual void SetupPlayerInputComponent(class UInputComponent* InPlayerInputComponent) override;

	virtual void Tick(float InDeltaSeconds) override;

	void SetMovementForAiming() const;
	void ResetMovement() const;

	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayAnimMontage(UAnimMontage* InMontageToPlay);
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_PauseAnimInstance() const;

	UFUNCTION(NetMulticast, Reliable)
	void MC_ResumeAnimInstance() const;

	UFUNCTION(Server, Reliable)
	void SR_FinishCollecting(ACollectibleItem* InCollectedItem);

	void UpdateCraftingMenu() const { CraftingComp->UpdateCraftingAvailabilities(); }

	void ShowDamageDealt(const float InDealtDamage) const;

	UFUNCTION(Client, Reliable)
	void CL_ShowDamageDealt(const float InDealtDamage) const;

	UFUNCTION(BlueprintCallable)
	EWeaponType GetEquippedWeaponType() const;

	UCameraComponent* GetCameraComp() const { return CameraComp; }

	UAnimInstance* GetAnimInstance() const;

	float GetHealth() const;
	
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
	float GetStaminaPercent() const;

	bool GetIsInCounterFrame() const { return bIsInCounterFrame; }

	float GetCounterReduction() const { return CounterReduction; }

	UFUNCTION(Client, Reliable)
	void CL_StopRecoverHealth();

	UFUNCTION(Client, Reliable)
	void CL_SetRecoverableHealth(float InRecoverableHealth);

	UFUNCTION(BlueprintCallable)
	float GetRecoverableHealthPercent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHealthChanged();
	
	void UpdateCameraFOV(float InDeltaSeconds);

	UFUNCTION(Server, Unreliable)
	void SR_UpdateAimingRotation(const FRotator& InNewRotation);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SR_EquipWeapon(AActor* InWeaponOwner, const FName& InWeaponID);

	UFUNCTION()
	void ShowInteractingUI(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, class UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult);

	UFUNCTION()
	void HideInteractingUI(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex);
	
	void MoveVertical(float InAxisValue);
	void MoveHorizontal(float InAxisValue);

	void StartSprinting();
	void StopSprinting();

	void OnSpaceActionsPressed();

	UFUNCTION(Server, Reliable)
	void SR_DodgeRoll();

	UFUNCTION(Server, Reliable)
	void SR_DoSideStep(bool bInIsLeft);
	
	UFUNCTION(Server, Reliable)
	void SR_PutWeaponAway();

	void Interact();

	void ToggleCraftMenu();

	void OnWheelAxisChanged(float InAxisValue);

	UFUNCTION(BlueprintCallable)
	void ActivateInvincibility();

	UFUNCTION(BlueprintCallable)
	void DeactivateInvincibility();

	UFUNCTION(BlueprintCallable)
	bool GetInvincibility() const;

	UFUNCTION(BlueprintCallable)
	float GetChargeAmount() const;

	UFUNCTION(BlueprintCallable)
	bool CheckCounterAttack();

	UFUNCTION(BlueprintCallable)
	void StartRecoverHealth();

	UFUNCTION()
	void RecoverHealth();
	
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsInCounterFrame;

	UPROPERTY(EditAnywhere)
	float CounterReduction;

	float RecoverableHealth;
	float HealthRecoveryAmount;
	bool bCanStartRecoveryHealth;
	FTimerHandle HealthRecoveryTimerHandle;

	float DefaultFOV;
	float AimingMovingSpeed;
	float DefaultMovingSpeed;
};
