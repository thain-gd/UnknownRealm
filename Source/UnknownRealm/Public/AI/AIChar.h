// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "Core/MPCharacter.h"
#include "GameFramework/Character.h"
#include "AIChar.generated.h"

class APlayerCharacter;
class UHealthComponent;
class UBehaviorTree;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	ApproachVillage,
	AttackVillage,
	EncounterPlayer,
};

UCLASS()
class UNKNOWNREALM_API AAIChar : public AMPCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIChar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool HasAttackablePlayer() const { return TargetablePlayers.Num() > 0; }

	APlayerCharacter* RemoveFirstTargetPlayer();

	UBehaviorTree* GetCombatTree() const { return CombatTree; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnGotHit(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser);
	
	UFUNCTION()
	void OnHealthChanged();
	
	void Die();
	

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UBehaviorTree* CombatTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float AttackRange;

	UPROPERTY(BlueprintReadWrite)
	TSet<APlayerCharacter*> TargetablePlayers;
};
