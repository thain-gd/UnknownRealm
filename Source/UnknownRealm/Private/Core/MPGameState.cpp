#include "Core/MPGameState.h"

#include "Core/MPPlayerController.h"
#include "Net/UnrealNetwork.h"

void AMPGameState::SetWaveStatus(bool Won)
{
	bIsWon = Won;
	OnRep_WaveStatusChanged();
}

void AMPGameState::OnRep_WaveStatusChanged() const
{
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->ShowWaveResult();
	}
}

void AMPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPGameState, RemainingPreparingTime);
	DOREPLIFETIME(AMPGameState, bIsWon);
}
