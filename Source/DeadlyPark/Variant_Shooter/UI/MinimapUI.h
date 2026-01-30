
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapUI.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class DEADLYPARK_API UMinimapUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UImage* MinimapImage;
};
