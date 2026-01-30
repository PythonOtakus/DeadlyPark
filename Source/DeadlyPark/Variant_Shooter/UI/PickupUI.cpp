

#include "PickupUI.h"
#include "Components/TextBlock.h"
#include "Inventory/Equipment.h"
#include "Utility/Common.h"

#define LOCTEXT_NAMESPACE "Default"

void UPickupUI::UpdatePickup(AEquipment* Equipment)
{
	if (Equipment)
	{
		EquipmentName->SetText(Equipment->GetEquipmentData()->Name);
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

#undef LOCTEXT_NAMESPACE