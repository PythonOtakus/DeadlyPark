
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapCapture.generated.h"

UCLASS()
class DEADLYPARK_API AMinimapCapture : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
	TObjectPtr<USceneCaptureComponent2D> SceneCapture;
	
	UPROPERTY(EditAnywhere, Category = "Minimap")
	float CaptureHeight = 200.0f;

	AMinimapCapture();
	
	virtual void CaptureAround(AActor* Target);
};
