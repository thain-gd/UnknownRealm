#include "Core/MPGameMode.h"

#include "Core/MPGameController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MPPlayerController.h"
#include "Core/MPGameState.h"
#include "Core/MPPlayerState.h"

void AMPGameMode::PostLogin(APlayerController* NewPlayer)
{
	bool b_ValidGameController = IsValid(GameController);
	if (!b_ValidGameController)
	{
		b_ValidGameController = GetGameController();
	}
	
	if (b_ValidGameController && HasAuthority())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
			if (PlayerStart != nullptr)
			{
				StartLocations.AddUnique(PlayerStart);
			}
		}
	}

	AMPPlayerController* PlayerController = Cast<AMPPlayerController>(NewPlayer);
	if (!PlayerController)
		return;
	
	if (HasAuthority())
	{
		ClientRespawnPlayer(PlayerController);
	}

	GameController->ServerAddPlayer(PlayerController);
}

bool AMPGameMode::GetGameController()
{
	AMPGameController* FoundGameController = Cast<AMPGameController>(UGameplayStatics::GetActorOfClass(GetWorld(), AMPGameController::StaticClass()));
	if (IsValid(FoundGameController))
	{
		GameController = FoundGameController;
		return true;
	}

	return false;
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
	
	APawn* ToPossessPawn = GetWorld()->SpawnActor<APawn>(SpawnCharClass, StartLocations[SpawnPlayerCount]->GetActorTransform());
	PlayerController->Possess(ToPossessPawn);
	++SpawnPlayerCount;
}
