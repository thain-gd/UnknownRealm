#include "Core/MPGameMode.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MPPlayerController.h"
#include "Core/MPGameState.h"
#include "Core/MPPlayerState.h"

AMPGameMode::AMPGameMode()
	: MaxPreparingTime(600)
{
}

void AMPGameMode::PostLogin(APlayerController* NewPlayer)
{
	if (!HasAuthority())
		return;

	InitSpawnLocations();
	
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(NewPlayer);
	if (!PlayerController)
		return;
	
	ClientRespawnPlayer(PlayerController);
	PlayerController->ClientPostLogin();
}

void AMPGameMode::BeginPlay()
{
	AMPGameState* MPGameState = GetGameState<AMPGameState>();
	if (ensureAlways(MPGameState))
		MPGameState->InitPreparingTime(MaxPreparingTime);
	
	GetWorldTimerManager().SetTimer(PreparingTimeHandle, this, &AMPGameMode::UpdateRemainingTime, 1.f, true, 1.f);	
}

void AMPGameMode::InitSpawnLocations()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
		if (PlayerStart != nullptr)
		{
			SpawnLocations.AddUnique(PlayerStart);
		}
	}

	verify(SpawnLocations.Num() > 0);
}

void AMPGameMode::UpdateRemainingTime()
{
	float RemainingTime = GetWorldTimerManager().GetTimerRemaining(PreparingTimeHandle);

	AMPGameState* MPGameState = GetGameState<AMPGameState>();
	if (ensureAlways(MPGameState))
	{
		MPGameState->DecreaseRemainingPreparingTime();
		if (MPGameState->GetRemainingPreparingTime() <= 0)
		{
			GetWorldTimerManager().ClearTimer(PreparingTimeHandle);
			StartWave();
		}
	}
}

void AMPGameMode::StartWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave has started!"));
}

void AMPGameMode::ClientRespawnPlayer_Implementation(APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("RespawnPlayer"));
	APlayerState* PlayerState = PlayerController->PlayerState;
	APawn* ControlledPawn = PlayerController->GetPawn();
	if (IsValid(ControlledPawn))
	{
		GetWorld()->DestroyActor(ControlledPawn);
	}
	
	APawn* ToPossessPawn = GetWorld()->SpawnActor<APawn>(SpawnCharClass, SpawnLocations[SpawnedPlayerCount]->GetActorTransform());
	PlayerController->Possess(ToPossessPawn);
	++SpawnedPlayerCount;
}
