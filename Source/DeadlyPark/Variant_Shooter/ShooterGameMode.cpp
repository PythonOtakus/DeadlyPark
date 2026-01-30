

#include "Variant_Shooter/ShooterGameMode.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"
#include "ShooterUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Utility/Common.h"

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	ShooterUI = UCommon::CreatePlayerWidget<UShooterUI>(ShooterUIClass, EUIOrder::Dialog);
}

FString AShooterGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	if (AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(NewPlayerController))
	{
		FString PlayerName = UGameplayStatics::ParseOption(Options, TEXT("PlayerName")).TrimStartAndEnd();
		if (PlayerName.IsEmpty() || PlayerControllers.Contains(PlayerName))
		{
			return Result;
		}
		PlayerController->SetPlayerName(PlayerName);
		PlayerControllers.Add(PlayerName, PlayerController);
	}
	return Result;
}

void AShooterGameMode::IncrementTeamScore(uint8 TeamByte)
{
	int32 Score = 0;
	if (int32* FoundScore = TeamScores.Find(TeamByte))
	{
		Score = *FoundScore;
	}

	++Score;
	TeamScores.Add(TeamByte, Score);

	ShooterUI->BP_UpdateScore(TeamByte, Score);
}
