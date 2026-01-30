

#include "InventoryUI.h"
#include "EquipmentItemUI.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Inventory/InventoryComponent.h"
#include "Utility/Common.h"

void UInventoryUI::SetInventory(UInventoryComponent* Inventory = nullptr)
{
	if (Inventory) InventoryCache = Inventory;
	
	NearbyEquipments->ClearChildren();
	for (AEquipment* Equipment : InventoryCache->DetectNearbyEquipments())
	{
		UEquipmentItemUI* EquipmentItem = AddEquipmentItem(NearbyEquipments, Equipment);
		EquipmentItem->OnEquipmentItemMoved.AddDynamic(this, &UInventoryUI::AddEquipment);
	}
	
	InventoryEquipments->ClearChildren();
	for (auto& Pair : InventoryCache->GetEquipmentCounts())
	{
		UEquipmentItemUI* EquipmentItem = AddEquipmentItem(InventoryEquipments, Pair.Key, Pair.Value);
		EquipmentItem->OnEquipmentItemMoved.AddDynamic(this, &UInventoryUI::DiscardEquipment);
	}
}

UEquipmentItemUI* UInventoryUI::AddEquipmentItem(UScrollBox* ScrollBox, AEquipment* Equipment)
{
	return AddEquipmentItem(ScrollBox, Equipment->GetClass(), Equipment->Count, Equipment);
}

UEquipmentItemUI* UInventoryUI::AddEquipmentItem(UScrollBox* ScrollBox, TSubclassOf<AEquipment> EquipmentClass, int32 Count, AEquipment* Equipment)
{
	UEquipmentItemUI* EquipmentItem = CreateWidget<UEquipmentItemUI>(this, EquipmentItemUIClass);
	UScrollBoxSlot* EquipmentSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(EquipmentItem));
	EquipmentSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
	EquipmentSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 5.0f));
	EquipmentSlot->SetHorizontalAlignment(HAlign_Fill);
	EquipmentSlot->SetVerticalAlignment(VAlign_Top);
	EquipmentItem->SetEquipment(EquipmentClass, Count, Equipment);
	return EquipmentItem;
}

UEquipmentItemUI* UInventoryUI::FindOrAddEquipmentItem(UScrollBox* ScrollBox, TSubclassOf<AEquipment> EquipmentClass, bool* bFound)
{
	UEquipmentItemUI* EquipmentItem = nullptr;
	if (bFound) *bFound = false;
	for (UWidget* Widget : ScrollBox->GetAllChildren())
	{
		if (UEquipmentItemUI* Item = Cast<UEquipmentItemUI>(Widget))
		{
			if (Item->GetEquipmentClass() == EquipmentClass)
			{
				EquipmentItem = Item;
				if (bFound) *bFound = true;
				break;
			}
		}
	}
	if (!EquipmentItem)
	{
		EquipmentItem = AddEquipmentItem(ScrollBox, EquipmentClass, 0);
	}
	return EquipmentItem;
}

void UInventoryUI::AddEquipment(UEquipmentItemUI* EquipmentItem)
{
	UCommon::Debug("UInventoryUI::AddEquipment()");
	int32 Count = InventoryCache->AddEquipment(EquipmentItem->GetEquipmentClass(), EquipmentItem->GetCount());
	if (Count >= EquipmentItem->GetCount())
	{
		EquipmentItem->GetEquipment()->Destroy();
		EquipmentItem->RemoveFromParent();
	}
	bool bFound;
	UEquipmentItemUI* NewEquipmentItem = FindOrAddEquipmentItem(InventoryEquipments, EquipmentItem->GetEquipmentClass(), &bFound);
	NewEquipmentItem->SetCount(NewEquipmentItem->GetCount() + Count);
	
	if (!bFound) NewEquipmentItem->OnEquipmentItemMoved.AddDynamic(this, &UInventoryUI::DiscardEquipment);
}

AEquipment* UInventoryUI::SpawnEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count)
{
	FVector Location = UCommon::GetMyLocation() + UCommon::GetMyForward() * (InventoryCache->EquipmentDetectionRadius * 0.75f);
	FRotator Rotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
	AEquipment* Equipment = GetWorld()->SpawnActor<AEquipment>(EquipmentClass, Location, Rotation);
	Equipment->SetActorScale3D(FVector(0.1f));
	Equipment->Count = Count;
	return Equipment;
}

void UInventoryUI::DiscardEquipment(UEquipmentItemUI* EquipmentItem)
{
	UCommon::Debug("UInventoryUI::DiscardEquipment()");
	int32 Count = InventoryCache->DiscardEquipment(EquipmentItem->GetEquipmentClass());
	UCommon::Debug(FString::Printf(TEXT("DiscardEquipment: Count = %d"), Count));
	if (Count <= 0) UCommon::Debug("Invalid Equipment Count");
	UEquipmentItemUI* NewEquipmentItem = AddEquipmentItem(NearbyEquipments, SpawnEquipment(EquipmentItem->GetEquipmentClass(), Count));
	NewEquipmentItem->OnEquipmentItemMoved.AddDynamic(this, &UInventoryUI::AddEquipment);
	EquipmentItem->RemoveFromParent();
}
