

#include "MarkPoint.h"
#include "MarkPointUI.h"
#include "Components/WidgetComponent.h"
#include "Utility/Common.h"


AMarkPoint::AMarkPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	MarkPointWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MarkPointWidget"));
	MarkPointWidget->SetupAttachment(RootComponent);
	MarkPointWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MarkPointWidget->SetWidgetClass(MarkPointUIClass);
}

void AMarkPoint::BeginPlay()
{
	Super::BeginPlay();
	
	MarkPointUI = Cast<UMarkPointUI>(MarkPointWidget->GetWidget());
	Deadline = GetWorld()->GetTimeSeconds() + MarkPointLifetime;
}

void AMarkPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float Distance = FVector::Distance(UCommon::GetMyLocation(), GetActorLocation());
	MarkPointUI->UpdateMarkPointText(Distance);
	if (GetWorld()->GetTimeSeconds() >= Deadline) Destroy();
}

