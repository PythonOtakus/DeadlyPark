
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentItemUI.generated.h"

class AEquipment;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentItemMoved, UEquipmentItemUI*, EquipmentItem);

/**
 * 
 */
UCLASS()
class DEADLYPARK_API UEquipmentItemUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UImage* EquipmentImage;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EquipmentName;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EquipmentCount;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<AEquipment> EquipmentClassCache;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	int32 CountCache;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	AEquipment* EquipmentCache;
	
public:
	
	UPROPERTY()
	FOnEquipmentItemMoved OnEquipmentItemMoved;
	
	TSubclassOf<AEquipment> GetEquipmentClass() const { return EquipmentClassCache; }
	
	int32 GetCount() const { return CountCache; }
	
	AEquipment* GetEquipment() const { return EquipmentCache; }
	
	void SetCount(int32 Count);
	
	void SetEquipment(AEquipment* Equipment);
	
	void SetEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count, AEquipment* Equipment = nullptr);
	
	UFUNCTION()
	void OnEquipmentImageLoaded(const FSoftObjectPath& AssetPath, UObject* Asset);
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void MoveEquipmentItem();
};
