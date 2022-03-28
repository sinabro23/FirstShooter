// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	// Ű����� ���⵹��. ���콺�� �׳� ��ǲ�� �ٷ� APawn::AddControllerYawInput �� �־��ٰ� �ٲ�(���ض�����)
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	// ���콺 ȸ���Լ�
	void Turn(float Value);
	void LookUp(float Value);

	void FIreWeapon();

	// �ѽ�� ���� �´� �����̼� ���ϴ� �Լ�
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	// �ܰ���
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float DeltaTime);

	// BaseTurnRate�� LookupRate�� �����ϴ��Լ� (�����Ҷ� ���ҋ��� ���ʹ�ư ������ ȭ��ǥ��ư (���콺x))
	void SetLookRates(); 

	// ũ�ν���� �������°� ���� �Լ�
	void CalculateCrosshairSpread(float DeltaTime);

	// �ݹ߽� ũ�ν���� ����
	void StartCrosshairBulletFire();
	UFUNCTION()
	void FinishCrosshairBulletFire();
	/////////////////////////


	/// �ڵ���� ���� �Լ�
	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	UFUNCTION()
	void AutoFireReset();
	//////////////////////////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:// �Ҵ���� �����ϸ� �� �������Ʈ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//ī�޶� ȸ�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate; // �ʴ� ��������� ī�޶� ���ư���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// �����Ҷ� ���Ҷ��� Ű�������Ű ���� �ٲٰ��ϱ�����(���콺x)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;
	////////////////////////////////////////////


	// ���콺 ���� ����
	// meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0")�� �����Ϳ��� �ִ밪 �ּҰ� �Ѱ������, ������â ��ũ���ϴ°��� �Ѱ谪�� ���ϱ����� UIMin = "1.0", UIMax = "1.0"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseHipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMAX = "1.0", UIMin = "1.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;
	///////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash; // �ѱ� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontage; // �ѽ�� �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles; // �� �´°� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles; // �Ѿ������� ��

	// ���ذ��� //////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAiming; // ��Ŭ�� ����
	float CameraDefaultFOV; // ���ؾ������� FOV, BeginPlay���� ������
	float CameraZoomedFOV; // ���������� FOV
	float CameraCurrentFOV; // ���Ҷ� õõ�� ī�޶� FOV�ٲ�� ���簪 ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed; // �� �ٲ�� �ӵ�
	/////////////////////////

	// ũ�ν���� ���� //////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier; // ȭ�� �߰������� ũ�ν���� ������������ ���������� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor; // ĳ���Ͱ� �̵� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor; // ĳ���Ͱ� ���� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor; // ĳ���Ͱ� ���� �� ���� ũ�ν��� ������ �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor; // ĳ���Ͱ� �ݹ� �� ���� ũ�ν��� ������ �� ��

	// �ݹ߽� ũ�ν���� ����
	float ShootTimeDuration; // �ݹ��� ���� �ð� ����Ʈ 0.05�� ���� ª������
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;
	//////////////////////////////////////


	// �ڵ���� ����
	bool bFireButtonPressed;
	bool bShouldFire; // ���� �������� �Ƚ�������
	float AutomaticFireRate; // ����ӵ�
	FTimerHandle AutoFireTimer; // �ݹ� ������ Ÿ�̸�
	//////////////////////////////


public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier();
};
