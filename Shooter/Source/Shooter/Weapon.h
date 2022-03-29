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
public:

	// 무기에 Impulse를 준다
	void ThrowWeapon();
	
};
