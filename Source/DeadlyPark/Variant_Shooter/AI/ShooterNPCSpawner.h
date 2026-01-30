
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterNPCSpawner.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class AShooterNPC;

/**
 *  NPC生成器，可以监听NPC的死亡，并定时生成新的NPC
 */
UCLASS()
class DEADLYPARK_API AShooterNPCSpawner : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* SpawnCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SpawnDirection;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner")
	TSubclassOf<AShooterNPC> NPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 10))
	float InitialSpawnDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 100))
	int32 SpawnCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 10))
	float RespawnDelay = 5.0f;

	FTimerHandle SpawnTimer;

public:
	AShooterNPCSpawner();

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

protected:
	void SpawnNPC();

	UFUNCTION()
	void OnNPCDied();
};
