
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupUI.generated.h"

class AEquipment;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DEADLYPARK_API UPickupUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EquipmentName;
	
public:
	
	UFUNCTION(BlueprintCallable, Category="Pickup")
	virtual void UpdatePickup(AEquipment* Equipment);
};
