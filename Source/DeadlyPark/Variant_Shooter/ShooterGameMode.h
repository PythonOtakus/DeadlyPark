
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

class AShooterPlayerController;
class AShooterCharacter;
class UShooterUI;

/**
 *  GameMode，处理服务器的游戏逻辑
 */
UCLASS(abstract)
class DEADLYPARK_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category="Shooter")
	TSubclassOf<UShooterUI> ShooterUIClass;

	TObjectPtr<UShooterUI> ShooterUI;

	TMap<uint8, int32> TeamScores;
	
	UPROPERTY(EditAnywhere, Category="Shooter")
	TSubclassOf<AShooterCharacter> CharacterClass;
	
	UPROPERTY(EditAnywhere, Category="Shooter")
	TArray<FTransform> SpawnPoints;
	
	UPROPERTY()
	TMap<FString, AShooterPlayerController*> PlayerControllers;

	virtual void BeginPlay() override;
	
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
		const FString& Options, const FString& Portal = L"") override;

public:
	void IncrementTeamScore(uint8 TeamByte);
};
