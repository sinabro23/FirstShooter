// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

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
	float CharacterYaw; // 현재프레임의 캐릭터의 Yaw
	float CharacterYawLastFrame; // 이전프레임의 캐릭터의 Yaw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	float RotationCurve; // 현재 프레임의 RotationCurve값 (Idle_Turn_90_Right_Trimmed 애니메이션에서 만들어준 커브의 값)
	float RotationCurveLastFrame; // 이전 프레임의 RotationCurve값 (Idle_Turn_90_Right_Trimmed 애니메이션에서 만들어준 커브의 값)
};
