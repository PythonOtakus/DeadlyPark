
#include "Variant_Shooter/Inventory/Equipment.h"
#include "Components/SphereComponent.h"
#include "Utility/Common.h"

TMap<TSubclassOf<AEquipment>, FEquipmentTableRow*> AEquipment::EquipmentDataMap;

// 构造函数, 可在此设置默认值
AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 创建碰撞球体作为根组件
	RootComponent = Collider = UCommon::CreateTrigger<USphereComponent>(this, ECC_Pawn);
	
	// 创建网格组件
	Mesh = UCommon::CreateEmptyMesh(Collider);
}

// 编辑器中修改时调用
void AEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (FEquipmentTableRow* EquipmentData = EquipmentType.GetRow<FEquipmentTableRow>(FString()))
	{
		// 设置网格组件的网格体
		Mesh->SetStaticMesh(EquipmentData->StaticMesh.LoadSynchronous());
	}
}

void AEquipment::BeginPlay()
{
	Super::BeginPlay();
	
	if (!EquipmentDataMap.Contains(GetClass()))
	{
		EquipmentDataMap.Add(GetClass(), EquipmentType.GetRow<FEquipmentTableRow>(FString()));
		UE_LOG(LogInit, Log, TEXT("@tan add %s from %p"),
			*GetClass()->GetName(),
			EquipmentType.GetRow<FEquipmentTableRow>(FString())
		);
	}
	else
	{
		UE_LOG(LogInit, Log, TEXT("@tan own %s from %p"),
			*GetClass()->GetName(),
			EquipmentDataMap[GetClass()]
		);
	}
}

FEquipmentTableRow* AEquipment::GetEquipmentData(TSubclassOf<AEquipment> EquipmentClass)
{
	return EquipmentDataMap.Contains(EquipmentClass)
		? EquipmentDataMap[EquipmentClass] : nullptr;
}
