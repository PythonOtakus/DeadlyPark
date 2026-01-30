

#include "Variant_Shooter/ShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ShooterCharacter.h"
#include "ShooterBulletCounterUI.h"
#include "DeadlyPark.h"
#include "InventoryUI.h"
#include "PickupUI.h"
#include "MinimapUI.h"
#include "LoginUI.h"
#include "ShooterGameMode.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "MarkPoint/MarkPoint.h"
#include "Utility/Common.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (ShouldUseTouchControls())
		{
			MobileControlsWidget = UCommon::CreatePlayerWidget<UUserWidget>(MobileControlsWidgetClass, EUIOrder::MobileControls);
		}

		BulletCounterUI = UCommon::CreatePlayerWidget<UShooterBulletCounterUI>(BulletCounterUIClass, EUIOrder::Background);
		
		MinimapUI = UCommon::CreatePlayerWidget<UMinimapUI>(MinimapUIClass, EUIOrder::Dialog);
		
		PickupUI = UCommon::CreatePlayerWidget<UPickupUI>(PickupUIClass, EUIOrder::Dialog);
		PickupUI->SetVisibility(ESlateVisibility::Hidden);
		
		InventoryUI = UCommon::CreatePlayerWidget<UInventoryUI>(InventoryUIClass);
		InventoryUI->SetVisibility(ESlateVisibility::Hidden);
		
		LoginUI = UCommon::CreatePlayerWidget<ULoginUI>(LoginUIClass, EUIOrder::Menu);
		LoginUI->StartButton->OnClicked.AddDynamic(this, &AShooterPlayerController::OnGameStarted);
		LoginUI->ExitButton->OnClicked.AddDynamic(this, &AShooterPlayerController::OnGameExited);
		bShowMouseCursor = true;
	}
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			AddInputMappingContext(CurrentContext, EInputPriority::GamePlay);
		}

		if (!ShouldUseTouchControls())
		{
			for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
			{
				AddInputMappingContext(CurrentContext, EInputPriority::GamePlay);
			}
		}
		
		AddInputMappingContext(LoginMappingContext, EInputPriority::Menu);
	}
}

bool AShooterPlayerController::AddInputMappingContext(UInputMappingContext* MappingContext, EInputPriority Priority)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (!InputPriorityStack.IsEmpty())
		{
			EInputPriority LastPriority = InputPriorityStack.Last().Priority;
			if (LastPriority > Priority) return false;
			
			if (LastPriority < Priority)
			{
				for (int32 Index = InputPriorityStack.Num() - 1; Index >= 0; --Index)
				{
					if (InputPriorityStack[Index].Priority < LastPriority) break;
					Subsystem->RemoveMappingContext(InputPriorityStack[Index].MappingContext);
				}
			}
		}
		
		Subsystem->AddMappingContext(MappingContext, static_cast<int32>(Priority));
		InputPriorityStack.Add(FInputPriority(MappingContext, Priority));
		return true;
	}
	
	return false;
}

bool AShooterPlayerController::RemoveInputMappingContext(UInputMappingContext* MappingContext)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputPriorityStack.IsEmpty()) return false;
		
		if (InputPriorityStack.Num() == 1)
		{
			if (InputPriorityStack[0].MappingContext != MappingContext) return false;
			Subsystem->RemoveMappingContext(MappingContext);
			InputPriorityStack.Empty();
			return true;
		}
	
		EInputPriority LastPriority = InputPriorityStack.Last().Priority;
		for (int32 Index = InputPriorityStack.Num() - 1; Index >= 0; --Index)
		{
			if (InputPriorityStack[Index].Priority < LastPriority) return false;
			
			if (InputPriorityStack[Index].MappingContext == MappingContext)
			{
				if (Index == InputPriorityStack.Num() - 1 && InputPriorityStack[Index-1].Priority < LastPriority)
				{
					LastPriority = InputPriorityStack[Index-1].Priority;
					for (int32 SubIndex = Index - 1; SubIndex >= 0; --SubIndex)
					{
						if (InputPriorityStack[SubIndex].Priority < LastPriority) break;
						Subsystem->AddMappingContext(InputPriorityStack[SubIndex].MappingContext,
							static_cast<int32>(InputPriorityStack[SubIndex].Priority));
					}
				}
				Subsystem->RemoveMappingContext(MappingContext);
				InputPriorityStack.RemoveAt(Index);
				return true;
			}
		}
	}
	
	return false;
}

EInputPriority AShooterPlayerController::GetActiveInputPriority() const
{
	return InputPriorityStack.IsEmpty() ? EInputPriority::GamePlay : InputPriorityStack.Last().Priority;
}

void AShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InPawn->OnDestroyed.AddDynamic(this, &AShooterPlayerController::OnPawnDestroyed);

	if (AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(InPawn))
	{
		ShooterCharacter->Tags.Add(PlayerPawnTag);

		ShooterCharacter->OnBulletCountUpdated.AddDynamic(this, &AShooterPlayerController::OnBulletCountUpdated);
		ShooterCharacter->OnDamaged.AddDynamic(this, &AShooterPlayerController::OnPawnDamaged);

		ShooterCharacter->OnDamaged.Broadcast(1.0f);
		
		ShooterCharacter->OnPickupUpdated.AddDynamic(this, &AShooterPlayerController::OnPickupUpdated);

		ShooterCharacter->OnInventoryShown.AddDynamic(this, &AShooterPlayerController::OnInventoryShown);
		ShooterCharacter->OnInventoryHidden.AddDynamic(this, &AShooterPlayerController::OnInventoryHidden);

		ShooterCharacter->OnPointMarked.AddDynamic(this, &AShooterPlayerController::OnPointMarked);
	}
}

void AShooterPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	if (IsValid(BulletCounterUI))
	{
		BulletCounterUI->UpdateBulletCounter(0, 0);
	}

	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		AActor* RandomPlayerStart = ActorList[FMath::RandRange(0, ActorList.Num() - 1)];

		const FTransform SpawnTransform = RandomPlayerStart->GetActorTransform();

		if (AShooterCharacter* RespawnedCharacter = GetWorld()->SpawnActor<AShooterCharacter>(CharacterClass, SpawnTransform))
		{
			Possess(RespawnedCharacter);
		}
	}
}

void AShooterPlayerController::OnBulletCountUpdated(int32 MagazineSize, int32 Bullets)
{
	if (BulletCounterUI)
	{
		BulletCounterUI->UpdateBulletCounter(MagazineSize, Bullets);
	}
}

void AShooterPlayerController::OnPawnDamaged(float LifePercent)
{
	if (IsValid(BulletCounterUI))
	{
		BulletCounterUI->BP_Damaged(LifePercent);
	}
}

bool AShooterPlayerController::ShouldUseTouchControls() const
{
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AShooterPlayerController::OnPickupUpdated(AEquipment* Equipment)
{
	PickupUI->UpdatePickup(Equipment);
}

void AShooterPlayerController::OnInventoryShown(UInventoryComponent* Inventory)
{
	InventoryUI->SetInventory(Inventory);
	InventoryUI->SetVisibility(ESlateVisibility::Visible);
	AddInputMappingContext(InventoryMappingContext, EInputPriority::UI);
	bShowMouseCursor = true;
}

void AShooterPlayerController::OnInventoryHidden(UInventoryComponent* Inventory)
{
	InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	RemoveInputMappingContext(InventoryMappingContext);
	bShowMouseCursor = false;
}

void AShooterPlayerController::OnPointMarked(const FVector& Point)
{
	GetWorld()->SpawnActor<AMarkPoint>(MarkPointClass, Point, FRotator::ZeroRotator);
}

void AShooterPlayerController::DoLogin(FString Name)
{
	PlayerName = Name;
	FString URL = FString::Printf(TEXT("127.0.0.1:8080?PlayerName=%s"), *Name);
	ClientTravel(URL, TRAVEL_Absolute);
}

void AShooterPlayerController::OnGameStarted()
{
	// DoLogin(LoginUI->NameTextBox->GetText().ToString());
	LoginUI->SetVisibility(ESlateVisibility::Hidden);
	RemoveInputMappingContext(LoginMappingContext);
	bShowMouseCursor = false;
}

void AShooterPlayerController::OnGameExited()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
