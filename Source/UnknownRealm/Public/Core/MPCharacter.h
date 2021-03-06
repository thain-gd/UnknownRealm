// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MPCharacter.generated.h"

UCLASS(Abstract)
class UNKNOWNREALM_API AMPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_PlayMontage(UAnimMontage* InMontage);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_PauseMontage(const UAnimMontage* InMontage = nullptr) const;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_ResumeMontage(const UAnimMontage* InMontage = nullptr) const;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_StopMontage(const UAnimMontage* InMontage = nullptr) const;

	UFUNCTION(BlueprintCallable)
	bool IsMontagePlaying(const UAnimMontage* InMontage) const;
	
	UFUNCTION(BlueprintCallable)
	bool IsAnyMontagePlaying() const;
	
	bool CheckMontageSection(const FName& InSection) const;

	UFUNCTION(Server, Reliable)
	void SR_SetMovementSpeed(const float NewSpeed) const;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_SetMovementSpeed(const float NewSpeed) const;
	
protected:
	UAnimInstance* GetAnimInstance() const;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float DefaultMoveSpeed;
};
