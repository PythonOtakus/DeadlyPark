
#pragma once

#include "CoreMinimal.h"
#include "WorldFolders.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Common.generated.h"

UENUM(BlueprintType)
enum class EUIOrder : uint8
{
	Background UMETA(DisplayName = "Background"),
	MobileControls UMETA(DisplayName = "MobileControls"),
	Dialog UMETA(DisplayName = "Dialog"),
	Panel UMETA(DisplayName = "Panel"),
	Default UMETA(DisplayName = "Default"),
	Menu UMETA(DisplayName = "Menu"),
	Popup UMETA(DisplayName = "Popup"),
	SubPopup UMETA(DisplayName = "SubPopup"),
	Max UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EInputPriority : uint8
{
	GamePlay UMETA(DisplayName = "GamePlay"),
	Mission UMETA(DisplayName = "Mission"),
	Dialog UMETA(DisplayName = "Dialog"),
	Panel UMETA(DisplayName = "Panel"),
	UI UMETA(DisplayName = "UI"),
	Menu UMETA(DisplayName = "Menu"),
	Popup UMETA(DisplayName = "Popup"),
	SubPopup UMETA(DisplayName = "SubPopup"),
	Max UMETA(DisplayName = "Max"),
};

/**
 * 通用函数
 */
UCLASS()
class DEADLYPARK_API UCommon : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Common")
	static APawn* GetMe()
	{
		return GWorld->GetFirstPlayerController()->GetPawn().Get();
	}
	
	UFUNCTION(BlueprintCallable, Category = "Common")
	static bool IsMe(AActor* Actor)
	{
		return Actor == GetMe();
	}
	
	UFUNCTION(BlueprintCallable, Category = "Common")
	static bool IsMyComponent(UActorComponent* Component)
	{
		return IsMe(Component->GetOwner());
	}
	
	UFUNCTION(BlueprintCallable, Category = "Common")
	static FVector GetMyLocation()
	{
		return GetMe()->GetActorLocation();
	}
	
	UFUNCTION(BlueprintCallable, Category = "Common")
	static FRotator GetMyRotation()
	{
		return GetMe()->GetActorRotation();
	}
	
	UFUNCTION(BlueprintCallable, Category = "Common")
	static FVector GetMyForward()
	{
		return GetMe()->GetActorForwardVector();
	}
	
	template <class T>
	static T* CreateTrigger(AActor* Owner, const ECollisionChannel Channel = ECC_Pawn)
	{
		auto Collider = static_cast<UPrimitiveComponent*>(Owner->CreateDefaultSubobject<T>(TEXT("Trigger")));
		Collider->SetCollisionProfileName(TEXT("Trigger"));
		return static_cast<T*>(Collider);
	}
	
	static UStaticMeshComponent* CreateEmptyMesh(USceneComponent* Parent)
	{
		auto Mesh = Parent->GetOwner()->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		Mesh->SetupAttachment(Parent);
		Mesh->SetCollisionProfileName(TEXT("NoCollision"));
		return Mesh;
	}
	
	template <class T>
	static T* CreatePlayerWidget(const TSubclassOf<UUserWidget> WidgetClass, const EUIOrder Order = EUIOrder::Default)
	{
		auto Widget = static_cast<UUserWidget*>(CreateWidget<T>(GWorld->GetFirstPlayerController(), WidgetClass));
		Widget->AddToPlayerScreen(static_cast<int32>(Order));
		return static_cast<T*>(Widget);
	}
	
	template <class T>
	static T* CreateSystemWidget(const TSubclassOf<UUserWidget> WidgetClass, const EUIOrder Order = EUIOrder::Default)
	{
		auto Widget = static_cast<UUserWidget*>(CreateWidget<T>(GWorld->GetFirstPlayerController(), WidgetClass));
		Widget->AddToViewport(static_cast<int32>(Order));
		return static_cast<T*>(Widget);
	}
	
private:
	
	static FCollisionQueryParams QueryParams;
	
public:
	
	static void IgnoreActor(const AActor* Actor);

	static TArray<FOverlapResult>& OverlapSphere(TArray<FOverlapResult>& Overlaps, const FVector& Center, const float Radius, const ECollisionChannel Channel = ECC_WorldDynamic);

	static TArray<FHitResult>& LineTrace(TArray<FHitResult>& Hits, const FVector& Start, const FVector& End, const ECollisionChannel Channel = ECC_WorldDynamic, bool Debug = false);

	static TArray<FHitResult>& ScreenCenterTrace(TArray<FHitResult>& Hits, const float Distance, const ECollisionChannel Channel = ECC_WorldDynamic, bool Debug = false);

	static TArray<FHitResult>& SweepSphere(TArray<FHitResult>& Hits, const FVector& Start, const FVector& End, const float Radius, const ECollisionChannel Channel = ECC_WorldDynamic);
	
	static constexpr float LoggingDisplayDuration = 8.0f;
	
	static void Debug(const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, LoggingDisplayDuration, FColor::Green, Message);
	}
	
	static void Warning(const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, LoggingDisplayDuration, FColor::Yellow, Message);
	}
	
	static void Error(const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, LoggingDisplayDuration, FColor::Red, Message);
	}
};
