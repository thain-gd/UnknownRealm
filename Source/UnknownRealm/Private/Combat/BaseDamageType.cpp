#include "Combat/BaseDamageType.h"

#include "Components/HealthComponent.h"
#include "Player/PlayerCharacter.h"


void UBaseDamageType::HandleDamage(float InDamage, AActor* InCauser, AActor* InReceiver) const
{
	UHealthComponent* ReceiverHealthComp = InReceiver->FindComponentByClass<UHealthComponent>();
	check(ReceiverHealthComp != nullptr);

	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(InReceiver))
	{
		PlayerChar->CL_StopRecoverHealth();
	}
	ReceiverHealthComp->TakeDamage(InDamage, InCauser, true);
}
