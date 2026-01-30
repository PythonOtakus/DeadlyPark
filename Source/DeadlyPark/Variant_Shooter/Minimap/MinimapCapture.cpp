

#include "MinimapCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Utility/Common.h"


AMinimapCapture::AMinimapCapture()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->CaptureSource = SCS_FinalColorLDR;
	SceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
}

void AMinimapCapture::CaptureAround(AActor* Target)
{
	FVector Location = Target->GetActorLocation();
	Location.Z = CaptureHeight;
	SetActorLocation(Location);
	FRotator Rotation = Target->GetActorRotation();
	Rotation.Pitch = -90.0f;
	SetActorRotation(Rotation);
	SceneCapture->CaptureScene();
}
