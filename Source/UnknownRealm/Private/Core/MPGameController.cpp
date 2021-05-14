#include "Core/MPGameController.h"
#include "Core/MPPlayerController.h"
#include "Core/MPPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"

// Sets default values
AMPGameController::AMPGameController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MaxPreparingTime = 600; // in seconds
}

void AMPGameController::BeginPlay()
{
	GetWorldTimerManager().SetTimer(PreparingTimeHandle, this, &AMPGameController::StartWave, 0.1f, false, MaxPreparingTime);
}

void AMPGameController::StartWave()
{
	
}

void AMPGameController::ServerAddPlayer_Implementation(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("AddPlayer"));

	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(NewPlayer);
	AMPPlayerState* PlayerState = Cast<AMPPlayerState>(PlayerController->PlayerState);
	const int32 PlayerId = PlayerState->GetPlayerId();
	if (PlayerId >= 0)
	{
		if (PlayerStates.IsValidIndex(PlayerId))
		{
			if (IsValid(PlayerStates[PlayerId]))
			{
				UKismetSystemLibrary::PrintString(GetWorld(), FString("EXISTS ALREADY"));
			}
			else
			{
				PlayerStates.Insert(PlayerState, PlayerId);
				PlayerControllers.Insert(PlayerController, PlayerId);
			}
		}
	}
}

