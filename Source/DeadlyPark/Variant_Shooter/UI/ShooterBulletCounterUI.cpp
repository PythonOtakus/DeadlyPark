

#include "ShooterBulletCounterUI.h"
#include "Components/TextBlock.h"

void UShooterBulletCounterUI::UpdateBulletCounter(int32 MagazineSize, int32 BulletCount)
{
	MagazineSizeText->SetText(FText::FromString(FString::FromInt(MagazineSize)));
	BulletCountText->SetText(FText::FromString(FString::FromInt(BulletCount)));
	if (BulletCount > 0)
	{
		BulletCountText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}
	else
	{
		BulletCountText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
}
