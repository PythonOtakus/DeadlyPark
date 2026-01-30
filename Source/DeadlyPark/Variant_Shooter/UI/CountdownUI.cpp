
#include "Variant_Shooter/UI/CountdownUI.h"
#include "Components/TextBlock.h"

// 默认的本地化命名空间
#define LOCTEXT_NAMESPACE "Default"

void UCountdownUI::StartCountdown(float Duration)
{
	bDuringCountdown = true;
	SetVisibility(ESlateVisibility::Visible);
	
	Deadline = GetWorld()->GetTimeSeconds() + Duration;
}

void UCountdownUI::StopCountdown()
{
	bDuringCountdown = false;
	SetVisibility(ESlateVisibility::Hidden);
}

void UCountdownUI::UpdateCountdownText(float RestTime) const
{
	FNumberFormattingOptions FormattingOptions;
    FormattingOptions.MinimumFractionalDigits = 2;	
	FormattingOptions.MaximumFractionalDigits = 2;
	CountdownText->SetText(
		FText::Format(LOCTEXT("Countdown", "Countdown: {0}"),
		FText::AsNumber(RestTime, &FormattingOptions)));
}

void UCountdownUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	StopCountdown();
}

void UCountdownUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bDuringCountdown)
	{
		const float RestTime = Deadline - GetWorld()->GetTimeSeconds();
		if (RestTime > 0.0f)
		{
			UpdateCountdownText(RestTime);
		}
		else
		{
			StopCountdown();
		}
	}
}

#undef LOCTEXT_NAMESPACE
