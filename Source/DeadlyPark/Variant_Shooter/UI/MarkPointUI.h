
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MarkPointUI.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class DEADLYPARK_API UMarkPointUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UImage* MarkPointImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MarkPointText;
	
	UFUNCTION(BlueprintCallable)
	void UpdateMarkPointText(float Distance);
};
