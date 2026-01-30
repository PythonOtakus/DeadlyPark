
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EvacuationPoint.generated.h"

class UCountdownUI;
class USphereComponent;

UCLASS()
class DEADLYPARK_API AEvacuationPoint : public AActor
{
	GENERATED_BODY()

	/** 碰撞球体, 本身可作为 Transform 使用 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* Collider;
	
	/** 撤离倒计时文本控件类 */
	UPROPERTY(EditAnywhere, Category="Evacuation", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountdownUI> CountdownClass;
	
	/** 撤离倒计时文本控件 */
	UPROPERTY()
	TObjectPtr<UCountdownUI> Countdown;
	
public:
	/** 撤离倒计时时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Evacuation")
	float CountdownDuration = 10.0f;
	
	/** 是否启用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Evacuation")
	bool bEnabled = false;
	
	// 构造函数, 可在此设置默认值
	AEvacuationPoint();

protected:
	// 游戏开始或 SpawnActor() 时调用
	virtual void BeginPlay() override;

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;
	
	// 角色进入碰撞体时调用
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	// 角色离开碰撞体时调用
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
