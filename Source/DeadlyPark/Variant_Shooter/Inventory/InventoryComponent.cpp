
#include "InventoryComponent.h"
#include "Equipment.h"
#include "Engine/OverlapResult.h"
#include "Utility/Common.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UInventoryComponent::AddEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count)
{
	if (!EquipmentCounts.Contains(EquipmentClass) && IsInventoryFull()) return 0;
	
	int32 OwnedCount = GetEquipmentCount(EquipmentClass);
	int32 Capacity = GetEquipmentCapacity(EquipmentClass);
	if (OwnedCount + Count > Capacity)
	{
		EquipmentCounts.FindOrAdd(EquipmentClass) = Capacity;
		return Capacity - OwnedCount;
	}
	EquipmentCounts.FindOrAdd(EquipmentClass) = OwnedCount + Count;
	return Count;
}

int32 UInventoryComponent::DiscardEquipment(TSubclassOf<AEquipment> EquipmentClass)
{
	if (EquipmentCounts.Contains(EquipmentClass))
	{
		int32 Count = EquipmentCounts[EquipmentClass];
		EquipmentCounts.Remove(EquipmentClass);
		return Count;
	}
	
	return 0;
}

int32 UInventoryComponent::UseEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count, bool bFullOrNone)
{
	if (!EquipmentCounts.Contains(EquipmentClass)) return 0;
	
	if (EquipmentCounts[EquipmentClass] >= Count)
	{
		EquipmentCounts[EquipmentClass] -= Count;
		if (EquipmentCounts[EquipmentClass] == 0) EquipmentCounts.Remove(EquipmentClass);
		return Count;
	}
	
	if (!bFullOrNone)
	{
		Count = EquipmentCounts[EquipmentClass];
		EquipmentCounts.Remove(EquipmentClass);
		return Count;
	}
	
	return 0;
}

const TArray<AEquipment*>& UInventoryComponent::DetectNearbyEquipments()
{
	NearbyEquipments.Reset();
	
	for (const FOverlapResult& Overlap : UCommon::OverlapSphere(NearbyOverlaps, GetOwner()->GetActorLocation(), EquipmentDetectionRadius))
	{
		if (AEquipment* Equipment = Cast<AEquipment>(Overlap.GetActor()))
		{
			NearbyEquipments.Add(Equipment);
		}
	}
	
	return NearbyEquipments;
}

AEquipment* UInventoryComponent::DetectAheadEquipment()
{
	UCommon::IgnoreActor(GetOwner());
	for (const FHitResult& Hit : UCommon::ScreenCenterTrace(AheadHits, EquipmentPickupDistance, ECC_WorldDynamic, true))
	{
		if (AEquipment* Equipment = Cast<AEquipment>(Hit.GetActor()))
		{
			return Equipment;
		}
	}
	
	return nullptr;
}
