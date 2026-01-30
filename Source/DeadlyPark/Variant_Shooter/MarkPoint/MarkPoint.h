
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MarkPoint.generated.h"

class UMarkPointUI;
class UWidgetComponent;

UCLASS()
class DEADLYPARK_API AMarkPoint : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "MarkPoint")
	TObjectPtr<UWidgetComponent> MarkPointWidget;
	
	UPROPERTY()
	TObjectPtr<UMarkPointUI> MarkPointUI;

public:
	UPROPERTY(EditAnywhere, Category = "MarkPoint")
	TSubclassOf<UMarkPointUI> MarkPointUIClass;
	
	UPROPERTY(EditAnywhere, Category = "MarkPoint")
	float MarkPointLifetime = 5.0f;
	
	AMarkPoint();

protected:
	float Deadline;
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
