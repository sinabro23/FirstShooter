// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8  //� ���ӿ������� ������� ����
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

	// ƽ�Լ�ó�� ������ ���� �Լ�, �ִϸ��̼� �������Ʈ���� �̺�Ʈ �׷����� BlueprintUpdateAnimation�� �ٿ���
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:

	// ���ڸ����� ���� �� ���� �Լ�
	void TurnInPlace();

	// �޸��µ��� ȸ���ϸ� ���Ը����
	void Lean(float DeltaTime);

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
	float TIPCharacterYaw; // ������������ ĳ������ Yaw, TIP(Turn In Place) , ���߿� ������������ ������Ʈ��
	float TIPCharacterYawLastFrame; // ������������ ĳ������ Yaw TIP(Turn In Place), ���߿� ������������ ������Ʈ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	float RotationCurve; // ���� �������� RotationCurve�� (Idle_Turn_90_Right_Trimmed �ִϸ��̼ǿ��� ������� Ŀ���� ��)
	float RotationCurveLastFrame; // ���� �������� RotationCurve�� (Idle_Turn_90_Right_Trimmed �ִϸ��̼ǿ��� ������� Ŀ���� ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	float Pitch; // �ѱ� ������ ������ AimOffset���� Pitch������ �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	bool bReloading; // �����ϴµ����� �ѱ� ������ �����ʰ� �ϱ�����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn in Place", Meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState; //� ���ӿ������� ������� ����

	// Leaning���� ó�� �߰�����
	FRotator CharacterRotation; // ���� ��������CharacterYaw
	FRotator CharacterRotationLastFrame;  // ���� ��������CharacterYaw
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lean", Meta = (AllowPrivateAccess = "true"))
	float YawDelta;	// �޸��� �ִϿ��̼� ���彺���̽����� ����� ��

	//Crouching
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	// �ѽ� �ݵ� Change the recoil weight based on turning in place and aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	float RecoilWeight; // �����Ҷ� �־��� ��

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	bool bTurningInPlace; //���ڸ� �����Ҷ� true

	//Equipping
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Crouch", Meta = (AllowPrivateAccess = "true"))
	bool bEquipping;


};
