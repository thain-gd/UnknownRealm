#include "Combat/BaseDamageType.h"

#include "Components/HealthComponent.h"


void UBaseDamageType::HandleDamage(float InDamage, AActor* InCauser, AActor* InReceiver) const
{
	UHealthComponent* ReceiverHealthComp = InReceiver->FindComponentByClass<UHealthComponent>();
	check(ReceiverHealthComp != nullptr);
	ReceiverHealthComp->TakeDamage(InDamage, InCauser, true);
}
