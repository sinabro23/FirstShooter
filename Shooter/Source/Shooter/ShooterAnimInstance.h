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

	// ƽ�Լ�ó�� ������ ���� �Լ�, �ִϸ��̼� �������Ʈ���� �̺�Ʈ �׷����� BlueprintUpdateAnimation�� �ٿ���
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:

	// ���ڸ����� ���� �� ���� �Լ�
	void TurnInPlace();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter; // ĳ���� ������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float Speed; // ���� ĳ���� �ӷ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating; // ĳ���Ͱ� �����̰� �ִ��� �ƴ���

	// ĳ���� ���� ���������� �̵��Ҷ� offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw; // ���⶧ offset�� 0�� �ż� ������ ���ߴ� �ִϸ��̼Ǹ� ������� �װ� �����ϱ����� ������ �����°� ������ ���ο� ����.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	bool bAiming; // �����ϰ��ִ����ƴ���


	// ���ڸ����� ���콺 ������ ĳ���� ������� �ϱ����� ����
	float CharacterYaw; // ������������ ĳ������ Yaw
	float CharacterYawLastFrame; // ������������ ĳ������ Yaw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	float RotationCurve; // ���� �������� RotationCurve�� (Idle_Turn_90_Right_Trimmed �ִϸ��̼ǿ��� ������� Ŀ���� ��)
	float RotationCurveLastFrame; // ���� �������� RotationCurve�� (Idle_Turn_90_Right_Trimmed �ִϸ��̼ǿ��� ������� Ŀ���� ��)
};
