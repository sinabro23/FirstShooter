// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon() :
	ThrowWeaponTime(0.7f),
	bFalling(false)
{
	// Tick�Լ� ����Ƿ��� �߰��ؾ���
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���Ⱑ ���� ������ �����ϰ� �ϱ�����
	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation = FRotator(0.0f, GetItemMesh()->GetComponentRotation().Yaw, 0.0f);
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AWeapon::ThrowWeapon()
{
	// �޽� Yaw�����̼� �������� Impulse�ٰ���
	FRotator MeshRotation = FRotator(0.0f, GetItemMesh()->GetComponentRotation().Yaw, 0.0f);
	// �޽��� MeshRotation�� �����̼� �ٲ� (pitch�� roll���� ������)
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward = GetItemMesh()->GetForwardVector();
	const FVector MeshRight = GetItemMesh()->GetRightVector();

	// ���⸦ ������ ���� (���ذ� �߾ȵ�)
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	float RandomRotation = FMath::FRandRange(0.0f, 30.f);
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDirection *= 10'000.f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_PickUp);
}
