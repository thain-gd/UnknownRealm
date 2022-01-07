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
	void MC_PauseMontage(UAnimMontage* InMontage = nullptr) const;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_ResumeMontage(UAnimMontage* InMontage = nullptr) const;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MC_StopMontage(UAnimMontage* InMontage = nullptr) const;

	UFUNCTION(BlueprintCallable)
	bool IsMontagePlaying(UAnimMontage* InMontage) const;
	
	bool IsAnyMontagePlaying() const;
	bool CheckMontageSection(const FName& InSection) const;
	
protected:
	UAnimInstance* GetAnimInstance() const;
};
