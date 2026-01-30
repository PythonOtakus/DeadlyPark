
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class AEquipment;
class UEquipmentItemUI;
class UScrollBox;
class UTextBlock;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class DEADLYPARK_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NearbyTitle;
	
	UPROPERTY(meta=(BindWidget))
	UScrollBox* NearbyEquipments;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InventoryTitle;
	
	UPROPERTY(meta=(BindWidget))
	UScrollBox* InventoryEquipments;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<UEquipmentItemUI> EquipmentItemUIClass;
	
	UPROPERTY()
	UInventoryComponent* InventoryCache;
	
public:
	
	UInventoryComponent* GetInventory() const { return InventoryCache; }
	
	void SetInventory(UInventoryComponent* Inventory);
	
protected:
	
	UEquipmentItemUI* AddEquipmentItem(UScrollBox* ScrollBox, AEquipment* Equipment);
	
	UEquipmentItemUI* AddEquipmentItem(UScrollBox* ScrollBox, TSubclassOf<AEquipment> EquipmentClass, int32 Count, AEquipment* Equipment = nullptr);
	
	UEquipmentItemUI* FindOrAddEquipmentItem(UScrollBox* ScrollBox, TSubclassOf<AEquipment> EquipmentClass, bool* bFound = nullptr);
	
	UFUNCTION()
	void AddEquipment(UEquipmentItemUI* EquipmentItem);
	
	AEquipment* SpawnEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count);
	
	UFUNCTION()
	void DiscardEquipment(UEquipmentItemUI* EquipmentItem);
};
