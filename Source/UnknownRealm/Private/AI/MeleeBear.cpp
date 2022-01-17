// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeBear.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

AMeleeBear::AMeleeBear()
	: PounceAttackCooldown(10.0f), PounceAttackRange(320.0f), bIsPounceAttackReady(true)
{
}

void AMeleeBear::OnPounceAttackFinished()
{
	bIsPounceAttackReady = false;
	MC_SetMovementSpeed(DefaultMoveSpeed);

	// Set a timer to reset next pounce attack availability after the cooldown
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMeleeBear::AllowNextPounceAttack, PounceAttackCooldown);
	
	if (APlayerCharacter* PlayerCharacter = GetPlayerInRange(AttackRange))
	{
		PlayerCharacter->EnableImmobility();
		// TODO: Change this y rotation to a taken down animation
		const FRotator LyingDownRotation(90.0f, PlayerCharacter->GetActorRotation().Yaw, PlayerCharacter->GetActorRotation().Pitch);
		PlayerCharacter->MC_SetPlayerRotation(LyingDownRotation);
		
		if (PlayerCharacter != CurrentTargetPlayer)
		{
			TargetablePlayers.Add(CurrentTargetPlayer);
			CurrentTargetPlayer = PlayerCharacter;
		}
		
		bIsChomping = true;
	}
}

void AMeleeBear::AllowNextPounceAttack()
{
	bIsPounceAttackReady = true;
}

bool AMeleeBear::CanRun() const
{
	return bIsPounceAttackReady && !IsPlayerInRangeBounds(CurrentTargetPlayer, PounceAttackRange * 1.5f);
}

bool AMeleeBear::CanDoPounceAttack() const
{
	return IsPlayerInRangeBounds(CurrentTargetPlayer, PounceAttackRange);
}

void AMeleeBear::StartRunning() const
{
	MC_SetMovementSpeed(RunSpeed);
}

APlayerCharacter* AMeleeBear::PickNextTargetPlayer()
{
	bIsChomping = false;
	return Super::PickNextTargetPlayer();
}

void AMeleeBear::OnGotHit(AActor* InDamagedActor, float InDamage, const UDamageType* InDamageType, AController* InInstigatedBy, AActor* InDamageCauser)
{
	Super::OnGotHit(InDamagedActor, InDamage, InDamageType, InInstigatedBy, InDamageCauser);

	if (bIsChomping && CurrentTargetPlayer)
	{
		APlayerCharacter* PreviousTargetPlayer = CurrentTargetPlayer;
		PickNextTargetPlayer();
		if (!PreviousTargetPlayer->IsDead())
		{
			PreviousTargetPlayer->DisableImmobility();
			// TODO: Change this y rotation to a standing up animation
			PreviousTargetPlayer->MC_SetPlayerRotation(FRotator::ZeroRotator);
			
			TargetablePlayers.Add(PreviousTargetPlayer);
		}
	}
}

void AMeleeBear::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMeleeBear, bIsChomping);
}
