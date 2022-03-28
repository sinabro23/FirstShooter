// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (nullptr == ShooterCharacter)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}

	if (ShooterCharacter)
	{
		//ĳ���� ���ǵ� ��������
		FVector Velocity = ShooterCharacter->GetVelocity();
		Velocity.Z = 0;// ���� �ӷ��� ����
		Speed = Velocity.Size();

		// ���߿� ���ִ���
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		// �����̰��ִ���
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
			bIsAccelerating = true;
		else
			bIsAccelerating = false;

		// ĳ���� ���� ������ �̵��Ҷ� �ִϸ��̼� ����������[38��]
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation(); // ĳ���Ͱ� aim�ϴ� ����
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity()); // �����̴� ������ �����̼�
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw; // �η����̼��� Yaw����
		
		if (ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw; // ���߱� �������� �������� �����س���
		}
		//FString RotationMessage = FString::Printf(TEXT("Base Aim Rotation : %f"), AimRotation.Yaw);
		//FString MovementMessage = FString::Printf(TEXT("Movement Rotation : %f"), MovementRotation.Yaw);
		//FString OffsetMessage = FString::Printf(TEXT("MovementOffsetYaw : %f"), MovementOffsetYaw);
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::White, OffsetMessage);
		//}

		bAiming = ShooterCharacter->GetAiming();
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
