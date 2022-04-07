// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

UShooterAnimInstance::UShooterAnimInstance() :
	Speed(0.f),
	bIsInAir(false),
	bIsAccelerating(false),
	MovementOffsetYaw(0.f),
	LastMovementOffsetYaw(0.f),
	bAiming(false),
	TIPCharacterYaw(0.f),
	TIPCharacterYawLastFrame(0.f),
	CharacterRotation(FRotator::ZeroRotator),
	CharacterRotationLastFrame(FRotator::ZeroRotator),
	YawDelta(0.f),
	RootYawOffset(0.f),
	Pitch(0.f),
	bReloading(false),
	OffsetState(EOffsetState::EOS_Hip),
	bCrouching(false),
	RecoilWeight(1.f),
	bTurningInPlace(false),
	EQuippedWeaponType(EWeaponType::EWT_AssaultRifle),
	bShouldUseFABRIK(false)
{

}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (nullptr == ShooterCharacter)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}

	if (ShooterCharacter)
	{
		bCrouching = ShooterCharacter->GetCrouching();
		bReloading = (ShooterCharacter->GetCombatState() == ECombatState::ECS_Reloading);
		bEquipping = (ShooterCharacter->GetCombatState() == ECombatState::ECS_Equipping);
		bShouldUseFABRIK = (ShooterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied) || (ShooterCharacter->GetCombatState() == ECombatState::ECS_FireTimerInProgress);

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

		bAiming = ShooterCharacter->GetAiming();

		if (bReloading)
		{
			OffsetState = EOffsetState::EOS_Reloading;
		}
		else if (bIsInAir)
		{
			OffsetState = EOffsetState::EOS_InAir;
		}
		else if(ShooterCharacter->GetAiming())
		{
			OffsetState = EOffsetState::EOS_Aiming;
		}
		else
		{
			OffsetState = EOffsetState::EOS_Hip;
		}

		// Check if shootercharacter has a valid EquippedWeapon
		if (ShooterCharacter->GetEquippedWeapon())
		{
			EQuippedWeaponType = ShooterCharacter->GetEquippedWeapon()->GetWeaponType();
		}
	}

	TurnInPlace();
	Lean(DeltaTime);
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::TurnInPlace()
{
	if (!ShooterCharacter)
		return;

	Pitch = ShooterCharacter->GetBaseAimRotation().Pitch;

	if (Speed > 0 || bIsInAir) // 모든걸 리셋
	{
		// 움직임이 있을때는 제자리회전을 안함
		RootYawOffset = 0.f;
		TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
	}
	else
	{
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float TIPYawDelta = TIPCharacterYaw - TIPCharacterYawLastFrame;

		// RootYawOffsetFMF -180~180으로 한정
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

		// 1.0 이면 Turning, 0.0 이면 아니다 (Idle_Turn_90_Right_Trimmed애니메이션에서 커브에 Turning이란 이름으로 추가한 메타데이터 쭉 값이 1로돼있음)
		const float Turning = GetCurveValue(TEXT("Turning"));
		if (Turning > 0)
		{
			bTurningInPlace = true;
			RotationCurveLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("Rotation"));
			const float DeltaRotation = RotationCurve - RotationCurveLastFrame;

			// RootYawOffset > 0 이면 왼쪽으로 회전, RootYawOffset < 0 이면 오른쪽으로 회전
			RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

			const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
			if (ABSRootYawOffset > 90.f)
			{
				const float YawExcess{ ABSRootYawOffset - 90.f };
				RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
		else
		{
			bTurningInPlace = false;
		}
	}

	// RecoilWeight(반동) 설정
	if (bTurningInPlace ) // 제자리돌기할때
	{
		if (bReloading || bEquipping)
		{
			RecoilWeight = 1.f;
		}
		else
		{
			RecoilWeight = 0.f;
		}
	}
	else // 제자리돌기 안할때
	{
		if (bCrouching)
		{
			if (bReloading || bEquipping)
			{
				RecoilWeight = 1.f;
			}
			else
			{
				RecoilWeight = 0.1f;
			}
		}
		else
		{
			if (bAiming || bReloading || bEquipping)
			{
				RecoilWeight = 1.f;
			}
			else
			{
				RecoilWeight = 0.5f;
			}
		}
	}
}

void UShooterAnimInstance::Lean(float DeltaTime)
{
	if (!ShooterCharacter) return;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ShooterCharacter->GetActorRotation();

	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);

	const float Target = Delta.Yaw / DeltaTime; // / DeltaTime는 작은값이므로 Delta.Yaw값이 커짐
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f);
	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}
