// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNKNOWNREALM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveVertical(float AxisValue);
	void MoveHorizontal(float AxisValue);
	
	
private:
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComp;
	

};
