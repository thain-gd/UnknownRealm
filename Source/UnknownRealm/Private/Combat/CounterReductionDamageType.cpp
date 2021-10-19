#include "Combat/CounterReductionDamageType.h"

#include "Components/HealthComponent.h"
#include "Player/PlayerCharacter.h"

void UCounterReductionDamageType::HandleDamage(float InDamage, AActor* InCauser, AActor* InReceiver) const
{
	// This damage type must only be received by players
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(InReceiver);
	check(InReceiver != nullptr);
	
	// Always have 1HP remaining even the damage is greater than the current HP
	// when a counter succeeds (players can't die)
	const float ReducedDamage = InDamage - InDamage * PlayerChar->GetCounterReduction();
 	const float ActualDamage = ReducedDamage - PlayerChar->GetHealth() >= 0 ? PlayerChar->GetHealth() - 1 : ReducedDamage;
	if (ActualDamage != 0)
	{
		PlayerChar->CL_SetRecoverableHealth(ActualDamage);
	}
	PlayerChar->FindComponentByClass<UHealthComponent>()->TakeDamage(ActualDamage, InCauser);
}
