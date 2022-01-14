// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeBear.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

AMeleeBear::AMeleeBear()
	: ChargeAttackCooldown(10.0f), bIsChargeAttackReady(true)
{
}

void AMeleeBear::EnableChargeAttack()
{
	bIsChargeAttackReady = true;
	UpdateMoveSpeed();
}

void AMeleeBear::DisableChargeAttack()
{
	bIsChargeAttackReady = false;
	UpdateMoveSpeed();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMeleeBear::EnableChargeAttack, ChargeAttackCooldown);
}

void AMeleeBear::UpdateMoveSpeed()
{
	if (bIsChargeAttackReady)
	{
		MC_SetMovementSpeed(RunSpeed);
	}
	else
	{
		MC_SetMovementSpeed(DefaultMoveSpeed);
	}
}

void AMeleeBear::OnChargeAttackFinished()
{
	DisableChargeAttack();
	if (APlayerCharacter* PlayerCharacter = GetPlayerInRange(AttackRange))
	{
		CurrentTargetPlayer->GetCharacterMovement()->DisableMovement();
		// TODO: Change this y rotation to a taken down animation
		const FRotator LyingDownRotation(90.0f, CurrentTargetPlayer->GetActorRotation().Yaw, CurrentTargetPlayer->GetActorRotation().Pitch);
		CurrentTargetPlayer->SetActorRotation(LyingDownRotation);
		bIsChomping = true;
	}
}

bool AMeleeBear::CanDoChargeAttack() const
{
	return bIsChargeAttackReady && IsPlayerInRangeBounds(CurrentTargetPlayer, 330.0f, 250.0f);
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
			PreviousTargetPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			TargetablePlayers.Add(PreviousTargetPlayer);

			// TODO: Change this y rotation to a standing up animation
			PreviousTargetPlayer->SetActorRotation(FRotator::ZeroRotator);
		}
	}
}

void AMeleeBear::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMeleeBear, bIsChomping);
}
