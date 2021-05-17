#include "Core/MPGameState.h"
#include "Net/UnrealNetwork.h"

void AMPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPGameState, RemainingPreparingTime);
}
