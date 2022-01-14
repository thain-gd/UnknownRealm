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
class UBlackboardData;

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

	virtual APlayerCharacter* PickNextTargetPlayer();
	
	void CleanCurrentTargetPlayer() { CurrentTargetPlayer = nullptr; }

	bool CanDoBaseAttack() const;

	UBlackboardData* GetBlackboardData() const { return BlackboardData; }

	UBehaviorTree* GetCombatTree() const { return CombatTree; }

	APlayerCharacter* GetCurrentTargetPlayer() const { return CurrentTargetPlayer; }

	void StartAttacking() { bFinishedAttacking = false; }

	UFUNCTION(BlueprintCallable)
	void FinishAttacking() { bFinishedAttacking = true; }

	UFUNCTION(BlueprintCallable)
	bool IsFinishedAttacking() const { return bFinishedAttacking; }

	// TODO: Extract this to a utils namespace/class
	bool IsPlayerInRangeBounds(AActor* InPlayer, const float InUpperBound, const float InLowerBound = 0.0f) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplySimpleDamage(AActor* InDamagedActor, const float InDamage);

	UFUNCTION(BlueprintPure)
	APlayerCharacter* GetPlayerInRange(const float InRange);

	UFUNCTION()
	virtual void OnGotHit(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser);
	
private:
	UFUNCTION()
	void OnHealthChanged();
	
	void Die();
	

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | AI")
	UBlackboardData* BlackboardData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat | AI")
	UBehaviorTree* CombatTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackRange;

	UPROPERTY(BlueprintReadWrite)
	TSet<APlayerCharacter*> TargetablePlayers;

	UPROPERTY()
	APlayerCharacter* CurrentTargetPlayer;

	UPROPERTY(BlueprintReadWrite)
	bool bFinishedAttacking;
};
