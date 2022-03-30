// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	void StopFalling();

private:
	// 무기 던지는 것 관련 
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;
	////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo; // 이 무기의 총알수
public:
	// 무기에 Impulse를 준다
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	// 캐릭터가 총을 쏠때 실행될 함수(총알 감소)
	void DecrementAmmo();
	
};
