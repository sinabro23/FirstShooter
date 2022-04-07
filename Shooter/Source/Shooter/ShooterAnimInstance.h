// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponType.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8  //어떤 에임오프셋을 사용할지 결정
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),

	EOS_Max UMETA(DisplayName = "DefaultMAX"),
};

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterAnimInstance();

	// 틱함수처럼 쓰려고 만든 함수, 애니메이션 블루프린트에서 이벤트 그래프의 BlueprintUpdateAnimation에 붙였음
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:

	// 제자리에서 도는 것 관련 함수
	void TurnInPlace();

	// 달리는동안 회전하면 기울게만들기
	void Lean(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter; // 캐릭터 포인터

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float Speed; // 현재 캐릭터 속력

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating; // 캐리터가 움직이고 있는지 아닌지

	// 캐릭터 왼쪽 오른쪽으로 이동할때 offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw; // 멈출때 offset이 0이 돼서 앞으로 멈추는 애니메이션만 실행됐음 그걸 방지하기위해 마지막 오프셋값 저장할 새로운 변수.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bAiming; // 조준하고있는지아닌지


	// 제자리에서 마우스 돌릴때 캐릭터 따라오게 하기위한 변수
	float TIPCharacterYaw; // 현재프레임의 캐릭터의 Yaw, TIP(Turn In Place) , 공중에 있지않을때만 업데이트됨
	float TIPCharacterYawLastFrame; // 이전프레임의 캐릭터의 Yaw TIP(Turn In Place), 공중에 있지않을때만 업데이트됨
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	float RotationCurve; // 현재 프레임의 RotationCurve값 (Idle_Turn_90_Right_Trimmed 애니메이션에서 만들어준 커브의 값)
	float RotationCurveLastFrame; // 이전 프레임의 RotationCurve값 (Idle_Turn_90_Right_Trimmed 애니메이션에서 만들어준 커브의 값)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	float Pitch; // 총구 방향을 돌리는 AimOffset에서 Pitch값으로 쓸 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	bool bReloading; // 장전하는동안은 총구 방향이 돌지않게 하기위해

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState; //어떤 에임오프셋을 사용할지 결정

	// Leaning에서 처음 추가했음
	FRotator CharacterRotation; // 현재 프레임의CharacterYaw
	FRotator CharacterRotationLastFrame;  // 지난 프레임의CharacterYaw
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lean", Meta = (AllowPrivateAccess = "true"))
	float YawDelta;	// 달리는 애니에미션 블렌드스페이스에서 사용할 값

	//Crouching
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	// 총쏠때 반동 Change the recoil weight based on turning in place and aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	float RecoilWeight; // 블렌드할때 넣어줄 값

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	bool bTurningInPlace; //제자리 돌기할때 true

	//Equipping
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	bool bEquipping;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = "true"))
	EWeaponType EQuippedWeaponType; // 현재 착용중인 무기의 타입

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", Meta = (AllowPrivateAccess = "true"))
	bool bShouldUseFABRIK; //FABRIK쓰면 손위치 고정시킨데에서 움직이지않음. 장전할때 이상함

};
