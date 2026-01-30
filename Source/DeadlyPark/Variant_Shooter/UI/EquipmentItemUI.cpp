

#include "EquipmentItemUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Equipment.h"
#include "Utility/Common.h"

void UEquipmentItemUI::SetCount(int32 Count)
{
	CountCache = Count;
	EquipmentCount->SetText(FText::FromString(FString::FromInt(Count)));
}

void UEquipmentItemUI::SetEquipment(AEquipment* Equipment)
{
	SetEquipment(Equipment->GetClass(), Equipment->Count, Equipment);
}

void UEquipmentItemUI::SetEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count, AEquipment* Equipment)
{
	if (FEquipmentTableRow* EquipmentData = AEquipment::GetEquipmentData(EquipmentClass))
	{
		EquipmentClassCache = EquipmentClass;
		EquipmentCache = Equipment;
		EquipmentName->SetText(EquipmentData->Name);
		SetCount(Count);
		EquipmentData->Image.LoadAsync(
			FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &UEquipmentItemUI::OnEquipmentImageLoaded)
		);
	}
	else
	{
		UCommon::Debug("Invalid Equipment Data Row");
	}
}

void UEquipmentItemUI::OnEquipmentImageLoaded(const FSoftObjectPath& AssetPath, UObject* Asset)
{
	EquipmentImage->SetBrushFromTexture(Cast<UTexture2D>(Asset));
}

void UEquipmentItemUI::MoveEquipmentItem()
{
	UCommon::Debug("UEquipmentItemUI::MoveEquipmentItem()");
	OnEquipmentItemMoved.Broadcast(this);
}
