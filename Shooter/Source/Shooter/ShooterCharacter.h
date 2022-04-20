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
	ECS_Equipping UMETA(DisplayName = "Equipping"),

	ECS_MAX UMETA(DisplayName = "Default MAX")
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

		
public:

	// Interping할때 Location으로 활용할 것
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	// 이 SceneComponent에서 몇개나 인터핑할지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;
};

// 델레게이트로 현재 슬롯인덱스 보내기
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHighlightIconDelegate, int32, SlotIndex, bool, bStartAnimation);

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

	// 키보드로 방향돌기. 마우스는 그냥 인풋에 바로 APawn::AddControllerYawInput 로 넣었다가 바꿈(조준때문에)
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	// 마우스 회전함수
	void Turn(float Value);
	void LookUp(float Value);

	void FireWeapon();

	// 총쏘면 최종 맞는 로케이션 구하는 함수
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	// 줌관련
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float DeltaTime);

	// BaseTurnRate와 LookupRate를 설정하는함수 (조준할때 안할떄의 왼쪽버튼 오른쪽 화살표버튼 (마우스x))
	void SetLookRates(); 

	// 크로스헤어 벌어지는것 관련 함수
	void CalculateCrosshairSpread(float DeltaTime);

	// 격발시 크로스헤어 관련
	void StartCrosshairBulletFire();
	UFUNCTION()
	void FinishCrosshairBulletFire();
	/////////////////////////

	/// 자동사격 관련 함수
	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	UFUNCTION()
	void AutoFireReset();
	//////////////////////////

	/// 크로스헤어로부터 라인트레이스해서 충돌체크하는 함수
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	/// 오버랩된 아이템 개수(OverlappedItemCount)가 1개 이상이면 아이템 트레이스할 함수
	void TraceForItems();

	// 기본무기 스폰 
	class AWeapon* SpawnDefaultWeapon();

	// 무기장착해서 메쉬에 붙이기
	void EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping = false); 

	// 장착중인 무기 바닥으로 버리기
	void DropWeapon();

	// 'E'키 버튼
	void SelectButtonPressed();
	void SelectButtonReleased();

	// 현재 들고있는 무기 버리고 새로운 무기 주음
	void SwapWeapon(AWeapon* WeaponToSwap);

	//TMap<EAmmoType, int32> AmmoMap 초기화 BeginPlay에서 실행
	void InitializeAmmoMap();

	// 무기가 총알이 있는지 없는지
	bool WeaponHasAmmo();
	
	// 격발 관련 함수
	void PlayFireSound();
	void SendBullet();
	void PlayGunfireMontage();

	//Reload관련
	void ReloadButtonPressed(); // R키와 바인드할 함수
	void ReloadWeapon(); // 실제 리로드 하는 기능
	UFUNCTION(BlueprintCallable)
	void FinishReloading(); // 리로드 몽타주의 애님노티파이로 호출될 함수, AmmoMap 업데이트 할것
	bool CarryingAmmo(); // 현재 장착하고 있는 무기타입에 맞는 총알을 들고 있는지
	
	// 무기스왑
	UFUNCTION(BlueprintCallable)
	void FinishEquipping(); // Equipping Montage몽타주 애님노티파이로 호출될것

	// 리로드 몽타주 애님노피타이에서 호출할 함수
	UFUNCTION(BlueprintCallable)
	void GrabClip();
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	void CrouchButtonPressed();

	virtual void Jump() override;

	//쭈그리고 설때 캡슐크기 조절
	void InterpCapsuleHalfHeight(float DeltaTime);

	void Aim();
	void StopAiming();

	void PickupAmmo(class AAmmo* Ammo);

	void InitializeInterpLocations();
	
	//인벤관련()
	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();
	void FiveKeyPressed();
	void ExchangeInvetoryItems(int32 CurrentItemIndex, int32 NewItemIndex);
	int32 GetEmptyInventorySlot();

	void HighlightInventorySlot();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:// 할당들은 웬만하면 다 블루프린트에서 했음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//카메라 회전 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate; // 초당 어느정도로 카메라가 돌아갈지

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// 조준할때 안할때의 키보드방향키 감도 바꾸게하기위해(마우스x)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;
	////////////////////////////////////////////


	// 마우스 감도 관련
	// meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0")는 에디터에서 최대값 최소값 한계시켜줌, 디테일창 스크롤하는것의 한계값도 정하기위해 UIMin = "1.0", UIMax = "1.0"
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
	UAnimMontage* HipFireMontage; // 총쏘는 애니메이션 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles; // 총 맞는곳 파티클

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles; // 총알지나간 길

	// 조준관련 //////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAiming; // 우클릭 조준
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV; // 조준안했을때 FOV, BeginPlay에서 설정됨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CameraZoomedFOV; // 조준했을때 FOV
	float CameraCurrentFOV; // 줌할때 천천히 카메라 FOV바뀌게 현재값 저장 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed; // 줌 바뀌는 속도
	/////////////////////////

	// 크로스헤어 관련 //////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier; // 화면 중간점에서 크로스헤어 벌어짐을위해 최종적으로 더해줄 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor; // 캐릭터가 이동 할 때에 크로스헤어에 영향을 줄 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor; // 캐릭터가 점프 할 때에 크로스헤어에 영향을 줄 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor; // 캐릭터가 조준 할 때에 크로스헤어에 영향을 줄 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor; // 캐릭터가 격발 할 때에 크로스헤어에 영향을 줄 값

	// 격발시 크로스헤어 관련
	float ShootTimeDuration; // 격발후 지날 시간 디폴트 0.05로 아주 짧게했음
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;
	//////////////////////////////////////


	// 자동사격 관련
	bool bFireButtonPressed;
	bool bShouldFire; // 총을 쏴야할지 안쏴야할지
	FTimerHandle AutoFireTimer; // 격발 사이의 타이머
	//////////////////////////////

	// 아이템 트레이스 변수
	bool bShouldTraceForItems; //매프레임마다 아이템을 Trace해야하면 True
	int8 OverlappedItemCount; // 오버랩된 아이템 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLastFrame; 
	/////////////////////////////////////


	/// 무기 장착 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon; // 현재 캐릭터가 장착하고 있는 무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass; // 기본무기로 블루프린트에서 설정할것

	// 현재 TraceForItems함수로 인해 Trace된 아이템 저장 (nullptr일수 있음)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	// 무기획득시 카메라앞에 설 위치 정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float CameraInterpDistance; // 무기멈출 위치구할때 카메라의 Forward벡터에 곱해질값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation; // 무기멈출 위치구할때 카메라의 업벡터에 곱해질값.

	// 총알 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;//AmmoType에따른 총알 잔여량 Map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo; // 시작할때 9mm 총알량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;

	// combat state. UnOccupied 상태여야만 격발 이나 재장전 할 수 있음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	// 재장전 애님몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	// 무기 스왑 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

	// 재장전 탄창 버리고 끼는것
	// 재장전할때 처음 탄창을 잡을때의 탄창 트랜스폼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent; //리로드 할때 캐릭터 손에 붙일 SceneComponent

	// 쭈그리기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	// 움직임 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	// 쭈그릴때 화면및 캡슐크기조절
	float CurrentCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseGroundFriction; // 마찰력 낮은값이면 슬라이딩함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CrouchingGroundFriction;

	bool bAimingButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponInterpComp; // 아이템 획득시 여러방향에서 날라오게

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp1; // 아이템 획득시 여러방향에서 날라오게
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp2; // 아이템 획득시 여러방향에서 날라오게
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp3; // 아이템 획득시 여러방향에서 날라오게
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp4; // 아이템 획득시 여러방향에서 날라오게
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp5; // 아이템 획득시 여러방향에서 날라오게
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp6; // 아이템 획득시 여러방향에서 날라오게
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

	// 인벤토리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> Inventory; 
	const int32 INVENTORY_CAPACITY = 6; // 인벤 사이즈
	// 인벤토리의 SlotIndex를 보내줄 델레게이트
	UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = "true"))
	FEquipItemDelegate EquipItemDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = "true"))
	FHighlightIconDelegate HightlightIconDelegate; // Icon Animaion실행하기위한 Slot 정보를 건네주기위한 델레게이트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 HighlightedSlot; // 현재 하이라이트된 인벤 슬롯번호

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

	// 오버랩된 아이템 개수 설정 및 bShouldTraceForItem 설정 (오버랩된 아이템이 하나라도 있으면 Trace해야함)
	void IncrementOverlappedItemCount(int8 Amount);

	// 무기 획득시 카메라 앞에 설 위치
	//FVector GetCameraInterpLocation(); // 필요없어짐 Aitem 클래스에서 들고있음

	void GetPickupItem(AItem* Item);

	// Returns the index in InterpLocations array with the lowest ItemCount
	int32 GetInterpLocationIndex();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();

	void UnHighlightInventorySlot();

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
};
