
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownUI.generated.h"

class UTextBlock;

// 倒计时结束委托类
DECLARE_DELEGATE(FOnCountdownFinished);

/**
 * 撤离倒计时文本控件
 */
UCLASS()
class DEADLYPARK_API UCountdownUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	/** 倒计时文本控件 */
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountdownText;
	
	/** 倒计时结束委托 */
	FOnCountdownFinished OnCountdownFinished;
	
protected:
	
	/** 是否正在倒计时中 */
	bool bDuringCountdown;
	
	/** 倒计时结束时间 */
	float Deadline;
	
public:

	void StartCountdown(float Duration);
	
	void StopCountdown();
	
	bool IsDuringCountdown() const { return bDuringCountdown; }
	
	void UpdateCountdownText(float RestTime) const;
	
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
