

#include "LoginUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULoginUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	StartButton->OnHovered.AddDynamic(this, &ULoginUI::OnStartButtonHovered);
	StartButton->OnUnhovered.AddDynamic(this, &ULoginUI::OnStartButtonUnhovered);
}

void ULoginUI::OnStartButtonHovered()
{
	StartText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
}

void ULoginUI::OnStartButtonUnhovered()
{
	StartText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
}
