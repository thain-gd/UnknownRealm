#include "Core/MPGameState.h"

#include "Components/InventoryComponent.h"
#include "Core/MPPlayerController.h"
#include "Net/UnrealNetwork.h"

AMPGameState::AMPGameState()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	InventoryComp->SetIsReplicated(true);
	AddOwnedComponent(InventoryComp);
}

void AMPGameState::SetWaveStatus(bool Won)
{
	bIsWon = Won;

	MC_OnWaveStatusChanged();
}

void AMPGameState::AddToInventory(const FName& ItemID, const int32 Amount)
{
	InventoryComp->AddItem(ItemID, Amount);
}

void AMPGameState::OpenInventory() const
{
	InventoryComp->ShowWidget();
}

void AMPGameState::CloseInventory() const
{
	InventoryComp->HideWidget();
}

void AMPGameState::MC_OnWaveStatusChanged_Implementation() const
{
	UE_LOG(LogTemp, Warning, TEXT("MulticastOnWaveStatusChanged"));
	
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
	DOREPLIFETIME(AMPGameState, InventoryComp);
}
