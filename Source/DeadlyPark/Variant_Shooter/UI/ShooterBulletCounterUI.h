
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterBulletCounterUI.generated.h"

class UTextBlock;

/**
 *  
 */
UCLASS(abstract)
class DEADLYPARK_API UShooterBulletCounterUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* BulletCountText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MagazineSizeText;
	
public:

	UFUNCTION(BlueprintCallable, Category="Shooter", meta=(DisplayName = "UpdateBulletCounter"))
	void UpdateBulletCounter(int32 MagazineSize, int32 BulletCount);

	UFUNCTION(BlueprintImplementableEvent, Category="Shooter", meta=(DisplayName = "Damaged"))
	void BP_Damaged(float LifePercent);
};
