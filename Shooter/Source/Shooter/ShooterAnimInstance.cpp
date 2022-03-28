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
		//캐릭터 스피드 가져오기
		FVector Velocity = ShooterCharacter->GetVelocity();
		Velocity.Z = 0;// 공중 속력은 제거
		Speed = Velocity.Size();

		// 공중에 떠있는지
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		// 움직이고있는지
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
			bIsAccelerating = true;
		else
			bIsAccelerating = false;

		// 캐릭터 왼쪽 오른쪽 이동할때 애니메이션 설정때문에[38강]
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation(); // 캐릭터가 aim하는 방향
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity()); // 움직이는 방향의 로테이션
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw; // 두로테이션의 Yaw차이
		
		if (ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw; // 멈추기 전까지의 오프셋을 저장해놓음
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
