

#include "MarkPointUI.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "Default"

void UMarkPointUI::UpdateMarkPointText(float Distance)
{
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumFractionalDigits = 0;	
	FormattingOptions.MaximumFractionalDigits = 2;
	MarkPointText->SetText(
		FText::Format(LOCTEXT("Distance", "{0}m"),
		FText::AsNumber(Distance * 0.01f, &FormattingOptions)));
}

#undef LOCTEXT_NAMESPACE
