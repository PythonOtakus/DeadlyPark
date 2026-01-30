
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class AEquipment;
class USphereComponent;


/**
 *  持有掉落装备的表格行信息
 */
USTRUCT(BlueprintType)
struct FEquipmentTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	/** 掉落装备名称 */
	UPROPERTY(EditAnywhere)
	FText Name;

	/** 指定用于实例化的装备类 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEquipment> EquipmentClass;

	/** 掉落装备显示的网格体 */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> StaticMesh;
	
	/** 掉落装备在背包中的图片 */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Image;
};


UCLASS()
class DEADLYPARK_API AEquipment : public AActor
{
	GENERATED_BODY()

	/** 碰撞球体, 本身可作为 Transform 使用 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* Collider;

	/** 网格组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
protected:
	/** 掉落装备类型 */
	UPROPERTY(EditAnywhere, Category="Equipment")
	FDataTableRowHandle EquipmentType;
	
	static TMap<TSubclassOf<AEquipment>, FEquipmentTableRow*> EquipmentDataMap;
	
public:
	/** 掉落装备数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment")
	int32 Count = 1;
	
	/** 构造函数, 可在此设置默认值 */
	AEquipment();

protected:
	/** 编辑器中修改时调用 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** 启动时调用 */
	virtual void BeginPlay() override;
	
public:
	/** 获取掉落装备数据 */
	static FEquipmentTableRow* GetEquipmentData(TSubclassOf<AEquipment> EquipmentClass);
	
	FEquipmentTableRow* GetEquipmentData() const { return GetEquipmentData(GetClass()); }
};
