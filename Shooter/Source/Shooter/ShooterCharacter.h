// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoType.h"
#include "ShooterCharacter.generated.h"


UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName = "Default MAX")
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

		
public:

	// Interping�Ҷ� Location���� Ȱ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	// �� SceneComponent���� ��� ����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;
};

// ��������Ʈ�� ���� �����ε��� ������
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	// Ű����� ���⵹��. ���콺�� �׳� ��ǲ�� �ٷ� APawn::AddControllerYawInput �� �־��ٰ� �ٲ�(���ض�����)
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	// ���콺 ȸ���Լ�
	void Turn(float Value);
	void LookUp(float Value);

	void FireWeapon();

	// �ѽ�� ���� �´� �����̼� ���ϴ� �Լ�
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	// �ܰ���
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float DeltaTime);

	// BaseTurnRate�� LookupRate�� �����ϴ��Լ� (�����Ҷ� ���ҋ��� ���ʹ�ư ������ ȭ��ǥ��ư (���콺x))
	void SetLookRates(); 

	// ũ�ν���� �������°� ���� �Լ�
	void CalculateCrosshairSpread(float DeltaTime);

	// �ݹ߽� ũ�ν���� ����
	void StartCrosshairBulletFire();
	UFUNCTION()
	void FinishCrosshairBulletFire();
	/////////////////////////

	/// �ڵ���� ���� �Լ�
	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	UFUNCTION()
	void AutoFireReset();
	//////////////////////////

	/// ũ�ν����κ��� ����Ʈ���̽��ؼ� �浹üũ�ϴ� �Լ�
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	/// �������� ������ ����(OverlappedItemCount)�� 1�� �̻��̸� ������ Ʈ���̽��� �Լ�
	void TraceForItems();

	// �⺻���� ���� 
	class AWeapon* SpawnDefaultWeapon();

	// ���������ؼ� �޽��� ���̱�
	void EquipWeapon(AWeapon* WeaponToEquip); 

	// �������� ���� �ٴ����� ������
	void DropWeapon();

	// 'E'Ű ��ư
	void SelectButtonPressed();
	void SelectButtonReleased();

	// ���� ����ִ� ���� ������ ���ο� ���� ����
	void SwapWeapon(AWeapon* WeaponToSwap);

	//TMap<EAmmoType, int32> AmmoMap �ʱ�ȭ BeginPlay���� ����
	void InitializeAmmoMap();

	// ���Ⱑ �Ѿ��� �ִ��� ������
	bool WeaponHasAmmo();
	
	// �ݹ� ���� �Լ�
	void PlayFireSound();
	void SendBullet();
	void PlayGunfireMontage();

	//Reload����
	void ReloadButtonPressed(); // RŰ�� ���ε��� �Լ�
	void ReloadWeapon(); // ���� ���ε� �ϴ� ���
	UFUNCTION(BlueprintCallable)
	void FinishReloading(); // ���ε� ��Ÿ���� �ִԳ�Ƽ���̷� ȣ��� �Լ�, AmmoMap ������Ʈ �Ұ�
	bool CarryingAmmo(); // ���� �����ϰ� �ִ� ����Ÿ�Կ� �´� �Ѿ��� ��� �ִ���
	
	// ���ε� ��Ÿ�� �ִԳ���Ÿ�̿��� ȣ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void GrabClip();
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	void CrouchButtonPressed();

	virtual void Jump() override;

	//�ޱ׸��� ���� ĸ��ũ�� ����
	void InterpCapsuleHalfHeight(float DeltaTime);

	void Aim();
	void StopAiming();

	void PickupAmmo(class AAmmo* Ammo);

	void InitializeInterpLocations();
	
	//�κ�����()
	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();
	void FiveKeyPressed();
	void ExchangeInvetoryItems(int32 CurrentItemIndex, int32 NewItemIndex);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:// �Ҵ���� �����ϸ� �� �������Ʈ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//ī�޶� ȸ�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate; // �ʴ� ��������� ī�޶� ���ư���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// �����Ҷ� ���Ҷ��� Ű�������Ű ���� �ٲٰ��ϱ�����(���콺x)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;
	////////////////////////////////////////////


	// ���콺 ���� ����
	// meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0")�� �����Ϳ��� �ִ밪 �ּҰ� �Ѱ������, ������â ��ũ���ϴ°��� �Ѱ谪�� ���ϱ����� UIMin = "1.0", UIMax = "1.0"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;
	///////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash; // �ѱ� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontage; // �ѽ�� �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles; // �� �´°� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles; // �Ѿ������� ��

	// ���ذ��� //////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAiming; // ��Ŭ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV; // ���ؾ������� FOV, BeginPlay���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CameraZoomedFOV; // ���������� FOV
	float CameraCurrentFOV; // ���Ҷ� õõ�� ī�޶� FOV�ٲ�� ���簪 ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed; // �� �ٲ�� �ӵ�
	/////////////////////////

	// ũ�ν���� ���� //////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier; // ȭ�� �߰������� ũ�ν���� ������������ ���������� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor; // ĳ���Ͱ� �̵� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor; // ĳ���Ͱ� ���� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor; // ĳ���Ͱ� ���� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor; // ĳ���Ͱ� �ݹ� �� ���� ũ�ν��� ������ �� ��

	// �ݹ߽� ũ�ν���� ����
	float ShootTimeDuration; // �ݹ��� ���� �ð� ����Ʈ 0.05�� ���� ª������
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;
	//////////////////////////////////////


	// �ڵ���� ����
	bool bFireButtonPressed;
	bool bShouldFire; // ���� �������� �Ƚ�������
	float AutomaticFireRate; // ����ӵ�
	FTimerHandle AutoFireTimer; // �ݹ� ������ Ÿ�̸�
	//////////////////////////////

	// ������ Ʈ���̽� ����
	bool bShouldTraceForItems; //�������Ӹ��� �������� Trace�ؾ��ϸ� True
	int8 OverlappedItemCount; // �������� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLastFrame; 
	/////////////////////////////////////


	/// ���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon; // ���� ĳ���Ͱ� �����ϰ� �ִ� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass; // �⺻����� �������Ʈ���� �����Ұ�

	// ���� TraceForItems�Լ��� ���� Trace�� ������ ���� (nullptr�ϼ� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	// ����ȹ��� ī�޶�տ� �� ��ġ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float CameraInterpDistance; // ������� ��ġ���Ҷ� ī�޶��� Forward���Ϳ� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation; // ������� ��ġ���Ҷ� ī�޶��� �����Ϳ� ��������.

	// �Ѿ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;//AmmoType������ �Ѿ� �ܿ��� Map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo; // �����Ҷ� 9mm �Ѿ˷�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;

	// combat state. UnOccupied ���¿��߸� �ݹ� �̳� ������ �� �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	// ������ �ִԸ�Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	// ������ źâ ������ ���°�
	// �������Ҷ� ó�� źâ�� �������� źâ Ʈ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent; //���ε� �Ҷ� ĳ���� �տ� ���� SceneComponent

	// �ޱ׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	// ������ �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	// �ޱ׸��� ȭ��� ĸ��ũ������
	float CurrentCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseGroundFriction; // ������ �������̸� �����̵���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchingGroundFriction;

	bool bAimingButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponInterpComp; // ������ ȹ��� �������⿡�� �������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp1; // ������ ȹ��� �������⿡�� �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp2; // ������ ȹ��� �������⿡�� �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp3; // ������ ȹ��� �������⿡�� �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp4; // ������ ȹ��� �������⿡�� �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp5; // ������ ȹ��� �������⿡�� �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp6; // ������ ȹ��� �������⿡�� �������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInterpLocation> InterpLocations;
	
	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;

	void ResetPickupSoundTimer();
	void ResetEquipSoundTimer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float PickupSoundResetTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float EquipSoundResetTime;

	// �κ��丮
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> Inventory; 
	const int32 INVENTORY_CAPACITY = 6; // �κ� ������
	// �κ��丮�� SlotIndex�� ������ ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = "true"))
	FEquipItemDelegate EquipItemDelegate;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAiming() const { return bAiming; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE bool GetCrouching() const { return bCrouching; }
	FInterpLocation GetInterpLocation(int32 Index);

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier();

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	// �������� ������ ���� ���� �� bShouldTraceForItem ���� (�������� �������� �ϳ��� ������ Trace�ؾ���)
	void IncrementOverlappedItemCount(int8 Amount);

	// ���� ȹ��� ī�޶� �տ� �� ��ġ
	//FVector GetCameraInterpLocation(); // �ʿ������ Aitem Ŭ�������� �������

	void GetPickupItem(AItem* Item);

	// Returns the index in InterpLocations array with the lowest ItemCount
	int32 GetInterpLocationIndex();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();
};
