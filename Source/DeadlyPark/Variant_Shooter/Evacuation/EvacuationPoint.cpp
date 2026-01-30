

#include "EvacuationPoint.h"
#include "DeadlyParkCharacter.h"
#include "CountdownUI.h"
#include "Components/SphereComponent.h"
#include "Utility/Common.h"


// 实现构造函数, 设置默认值
AEvacuationPoint::AEvacuationPoint()
{
	// 每帧调用 Tick() 函数更新
	PrimaryActorTick.bCanEverTick = true;
	
	// 创建碰撞球体作为根组件
	RootComponent = Collider = UCommon::CreateTrigger<USphereComponent>(this, ECC_Pawn);
}

// 游戏开始或 SpawnActor() 时调用
void AEvacuationPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Countdown = UCommon::CreatePlayerWidget<UCountdownUI>(CountdownClass, EUIOrder::Dialog);
}

// 每帧调用
void AEvacuationPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 角色进入碰撞体时调用
void AEvacuationPoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (UCommon::IsMe(OtherActor) && IsValid(Countdown))
	{
		Countdown->StartCountdown(CountdownDuration);
	}
}

// 角色离开碰撞体时调用
void AEvacuationPoint::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (UCommon::IsMe(OtherActor) && IsValid(Countdown))
	{
		Countdown->StopCountdown();
	}
}
