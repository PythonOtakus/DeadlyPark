// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Utility/Common.h"
#include "ShooterPlayerController.generated.h"

class ULoginUI;
class AMarkPoint;
class UMinimapUI;
enum class EInputPriority : uint8;
enum class EInputType : uint8;
class AEquipment;
class UInventoryComponent;
class UInventoryUI;
class UPickupUI;
class UInputMappingContext;
class AShooterCharacter;
class UShooterBulletCounterUI;

USTRUCT()
struct FInputPriority
{
	GENERATED_BODY()
	
	UPROPERTY()
	UInputMappingContext* MappingContext;
	
	UPROPERTY()
	EInputPriority Priority;
	
	FInputPriority()
		: MappingContext(nullptr)
		, Priority(EInputPriority::GamePlay)
	{
	}
	
	FInputPriority(UInputMappingContext* InMappingContext, EInputPriority InPriority)
		: MappingContext(InMappingContext)
		, Priority(InPriority)
	{
	}
};

/**
 *  PlayerController，管理输入映射和玩家复活
 */
UCLASS(abstract, config="Game")
class DEADLYPARK_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;
	
	/** 背包界面输入映射 */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	UInputMappingContext* InventoryMappingContext;
	
	/** 登录界面输入映射 */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	UInputMappingContext* LoginMappingContext;
	
	/** 输入映射优先级栈 */
	UPROPERTY()
	TArray<FInputPriority> InputPriorityStack;

	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	UPROPERTY(EditAnywhere, Category="Shooter|Respawn")
	TSubclassOf<AShooterCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, Category="Shooter|UI")
	TSubclassOf<UShooterBulletCounterUI> BulletCounterUIClass;

	UPROPERTY(EditAnywhere, Category="Shooter|Player")
	FName PlayerPawnTag = FName("Player");
	
	UPROPERTY(EditAnywhere, Category="Shooter|Player")
	FString PlayerName;

	UPROPERTY()
	TObjectPtr<UShooterBulletCounterUI> BulletCounterUI;
	
	UPROPERTY(EditAnywhere, Category="Shooter|Minimap")
	TSubclassOf<UMinimapUI> MinimapUIClass;
	
	UPROPERTY()
	TObjectPtr<UMinimapUI> MinimapUI;
	
	UPROPERTY(EditAnywhere, Category="Shooter|Pickup")
	TSubclassOf<UPickupUI> PickupUIClass;
	
	UPROPERTY()
	TObjectPtr<UPickupUI> PickupUI;
	
	UPROPERTY(EditAnywhere, Category="Shooter|Inventory")
	TSubclassOf<UInventoryUI> InventoryUIClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryUI> InventoryUI;
	
	UPROPERTY(EditAnywhere, Category="Shooter|Login")
	TSubclassOf<ULoginUI> LoginUIClass;
	
	UPROPERTY()
	TObjectPtr<ULoginUI> LoginUI;
	
	UPROPERTY(EditAnywhere, Category="Shooter|MarkPoint")
	TSubclassOf<AMarkPoint> MarkPointClass;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	bool AddInputMappingContext(UInputMappingContext* MappingContext, EInputPriority Priority);
	
	bool RemoveInputMappingContext(UInputMappingContext* MappingContext);
	
public:
	
	/** 返回当前输入映射优先级 */
	EInputPriority GetActiveInputPriority() const;
	
	FString GetPlayerName() const { return PlayerName; }
	
	void SetPlayerName(FString Name) { PlayerName = Name; }
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnBulletCountUpdated(int32 MagazineSize, int32 Bullets);

	UFUNCTION()
	void OnPawnDamaged(float LifePercent);

	bool ShouldUseTouchControls() const;
	
	UFUNCTION()
	void OnPickupUpdated(AEquipment* Equipment);
	
	UFUNCTION()
	void OnInventoryShown(UInventoryComponent* Inventory);
	
	UFUNCTION()
	void OnInventoryHidden(UInventoryComponent* Inventory);
	
	UFUNCTION()
	void OnPointMarked(const FVector& Point);
	
	void DoLogin(FString PlayerName);
	
	UFUNCTION()
	void OnGameStarted();
	
	UFUNCTION()
	void OnGameExited();
};
