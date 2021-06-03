#include "Core/MPGameMode.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MPPlayerController.h"
#include "Core/MPGameState.h"
#include "Core/MPPlayerState.h"
#include "AIs/AIChar.h"

AMPGameMode::AMPGameMode()
	: MaxPreparingTime(600)
{
}

void AMPGameMode::PostLogin(APlayerController* NewPlayer)
{
	if (!HasAuthority())
		return;

	InitPlayerSpawnLocations();
	
	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(NewPlayer);
	if (!PlayerController)
		return;
	
	ClientRespawnPlayer(PlayerController);
	PlayerController->ClientPostLogin();
}

void AMPGameMode::BeginPlay()
{
	InitEnemySpawnLocations();

	AMPGameState* MPGameState = GetGameState<AMPGameState>();
	if (ensureAlways(MPGameState))
		MPGameState->InitPreparingTime(MaxPreparingTime);
	
	GetWorldTimerManager().SetTimer(PreparingTimeHandle, this, &AMPGameMode::UpdateRemainingTime, 1.f, true, 1.f);	
}

void AMPGameMode::InitEnemySpawnLocations()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("EnemySpawnLocation")), FoundActors);
	for (AActor* EnemySpawnLocation : FoundActors)
	{
		EnemySpawnLocations.Add(EnemySpawnLocation);
	}

	UE_LOG(LogTemp, Warning, TEXT("InitEnemySpawnLocations with %i"), EnemySpawnLocations.Num());
	verify(EnemySpawnLocations.Num() > 0);
}

void AMPGameMode::InitPlayerSpawnLocations()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
		if (PlayerStart != nullptr)
		{
			PlayerSpawnLocations.AddUnique(PlayerStart);
		}
	}

	verify(PlayerSpawnLocations.Num() > 0);
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

	for (AActor* SpawnLocationActor : EnemySpawnLocations)
	{
		GetWorld()->SpawnActor<AAIChar>(AIClasses[0], SpawnLocationActor->GetActorLocation(), FRotator::ZeroRotator);
		++EnemyCount;
	}
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
	
	APawn* ToPossessPawn = GetWorld()->SpawnActor<APawn>(SpawnCharClass, PlayerSpawnLocations[SpawnedPlayerCount]->GetActorTransform());
	PlayerController->Possess(ToPossessPawn);
	++SpawnedPlayerCount;
}

void AMPGameMode::OnEnemyDied()
{
	--EnemyCount;
	UE_LOG(LogTemp, Warning, TEXT("Remaining enemies: %i"), EnemyCount);
	if (EnemyCount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave is cleared!"));
		AMPGameState* MPGameState = GetGameState<AMPGameState>();
		if (MPGameState)
		{
			MPGameState->SetWaveStatus(true);
		}
	}
}

