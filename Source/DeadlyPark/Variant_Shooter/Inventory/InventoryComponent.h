
#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


/**
 * @class UInventoryComponent
 * @brief 用于管理装备的组件，支持添加、移除和使用装备。
 *
 * 该组件允许玩家或其他角色携带并管理多种类型的装备。通过调用相应的方法，可以增加或减少特定装备的数量，以及使用一定数量的装备。
 * 组件内的装备信息存储在一个映射中，键为装备类型（AEquipment的子类），值为装备的数量。
 *
 * @note 此组件设计为可从蓝图中实例化。
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADLYPARK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<TSubclassOf<AEquipment>, int32> EquipmentCounts;
	
	UPROPERTY()
	TMap<TSubclassOf<AEquipment>, int32> EquipmentCapacities;
	
	UPROPERTY()
	int32 InventoryCapacity = 9999;
	
	UPROPERTY()
	TArray<AEquipment*> NearbyEquipments;
	
	UPROPERTY()
	TArray<FOverlapResult> NearbyOverlaps;
	
	UPROPERTY()
	TArray<FHitResult> AheadHits;

public:
	/** 装备检测半径 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	float EquipmentDetectionRadius = 250.0f;
	
	/** 装备拾取距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	float EquipmentPickupDistance = 400.0f;
	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** 默认装备容量 */
	static constexpr int32 DefaultEquipmentCapacity = 9999;
	
	const TMap<TSubclassOf<AEquipment>, int32>& GetEquipmentCounts() const { return EquipmentCounts; }
	
	int32 GetEquipmentCount(TSubclassOf<AEquipment> EquipmentClass)
	{
		return EquipmentCounts.Contains(EquipmentClass) ? EquipmentCounts[EquipmentClass] : 0;
	}
	
	int32 GetEquipmentCapacity(TSubclassOf<AEquipment> EquipmentClass)
	{
		return EquipmentCapacities.Contains(EquipmentClass) ? EquipmentCapacities[EquipmentClass] : DefaultEquipmentCapacity;
	}
	
	bool IsInventoryFull() const { return EquipmentCounts.Num() >= InventoryCapacity; }
	
	/** 添加装备 */
	int32 AddEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count = 1);
	
	/** 丢弃装备 */
	int32 DiscardEquipment(TSubclassOf<AEquipment> EquipmentClass);

	/**
	 * @brief 使用指定数量的装备。
	 *
	 * 该方法尝试从库存中使用（减少）指定类型的装备。如果设置为bFullOrNone为true且请求的数量不可用，则不会使用任何装备并返回0。
	 * 如果未完全满足请求的数量但bFullOrNone为false，则使用所有可用的装备，并从库存中移除该项。
	 *
	 * @param EquipmentClass 要使用的装备类型。
	 * @param Count 请求使用的装备数量，默认值为1。
	 * @param bFullOrNone 如果为true，仅当可以完全满足请求的数量时才使用装备；否则，即使不能完全满足也会使用所有可用装备。默认值为false。
	 * @return 实际使用的装备数量。如果无法满足请求或库存中没有该装备，则返回0。
	 */
	int32 UseEquipment(TSubclassOf<AEquipment> EquipmentClass, int32 Count = 1, bool bFullOrNone = false);
	
	virtual const TArray<AEquipment*>& DetectNearbyEquipments();
	
	virtual AEquipment* DetectAheadEquipment();
};
