// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_UnCommon UMETA(DisplayName = "UnCommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),


	EIR_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_PickUp UMETA(DisplayName = "PickUp"), // �ٴڿ� �������ִ»���, �ݸ��� �������� ���� ������
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"), // ���� ȹ���ؼ� �������� ���� ����
	EIS_PickedUp UMETA(DisplayName = "PickedUp"), // ���� �� ��������� �ʰ� �κ����ִ»���
	EIS_Equipped UMETA(DisplayName = "Equipped"), // ���� �� ����ִ»���
	EIS_Falling UMETA(DisplayName = "Falling"), // ���� ������ �������»���

	EIS_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_AMMO UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),

	EIT_MAX UMETA(DisplayName = "Default MAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;
};

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetActiveStars(); // ��͵��� ���� �� ���� �ѱ�

	// Ammo�� SkeletaMesh�� ������� �ʾƼ� override�ؼ� ����ؾ���
	virtual void SetItemProperties(EItemState State); // ������Ʈ������ �������� ������Ʈ ���� ����, SetItemState���� ���̽����

	// InterpTimer�� �ð��Ǹ� ȣ���� �Լ�
	void FinishInterping();

	void ItemInterp(float DeltaTime); // Tick���� ȣ��� �Լ� binterping�� true�϶��� ����

	// Get interp location based on the item type
	FVector GetInterpLocation();

	void PlayPickupSound(bool bForcePlaySound = false);
	
	//Custom Depth����(���ظ���)
	virtual void InitializeCustomDepth();

	//����x �������� Construction script���� ���
	virtual void OnConstruction(const FTransform& Transform) override;
	void UpdatePulse();
	void ResetPulseTimer();
	void StartPulseTimer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayEquipSound(bool bForcePlaySound = false); // ����ĳ���Ϳ��� ȣ���
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollsionBox; // ����Ʈ���̽������� �浹�ϸ� ������ HUD������ �ݸ���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere; // ������ ���� ����� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemMesh; // ���̷�Ż�޽�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget; // ������ �����ߴ� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName; // �Ⱦ������� �� ������ �̸� // Bind�ϴ°� �𸣸� 66��
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount; // �Ѿ� �������� ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity; // ������ ���� ������ ��͵�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars; // �������ѱ����� bool array

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState; // �������� ���� ����

	// Item Interp ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve; // ������ �����̿ö� z���� ���� Ŀ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation; // ������ ȹ��� ������ ���۵� �����̼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bInterping;

	FTimerHandle ItemInterpTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	// ������ �Ǵ� ������ x�� y�� ��
	float ItemInterpX;
	float ItemInterpY;

	float InterpInitialYawOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;
	//////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickupSound; // ������ �ֿﶧ ���� �Ҹ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound; // �����ɶ� ���� �Ҹ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	// index of the interp location this item is interping to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 InterpLocIndex;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* Character;

	// ���ؾȵȰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex; 	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;
	bool bCanChangeCustomDepth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveVector* PulseCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveVector* InterpPulseCurve;
	FTimerHandle PulseTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float PulseCurveTime;
	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float GlowAmount;
	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelExponent;
	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelReflectFraction;

	// �κ��丮
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex; // �κ��丮 �迭�� ���Թ�ȣ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	bool bCharacterInvetoryFull;

	//���������̺�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	UDataTable* ItemRarityDataTable;

	/** Color in the glow material */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;

	/** Light color in the pickup widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor LightColor;

	/** Dark color in the pickup widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor DarkColor;

	/** Number of stars in the pickup widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;

	/** Backgroud icon for the inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;




public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE UBoxComponent* GetCollsionBox() const { return CollsionBox; }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetSlotIndex(int32 Index) { SlotIndex = Index; }
	FORCEINLINE void SetCharacter(AShooterCharacter* Char) { Character = Char; }
	FORCEINLINE void SetCharacterInventoryFull(bool bFull) { bCharacterInvetoryFull = bFull; }

	void SetItemState(EItemState State); // SetItemProperties�� ������


	// ShooterCharacterŬ�������� ȣ���� �Լ�
	void StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound = false);

	//Custom Depth����(���ظ���)
	virtual void EnableCustomDepth();
	virtual void DisableCustomDepth();
	void EnableGlowMaterial();
	void DisableGlowMaterial();
};
