// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDOverlayClass; // �Ѿ� �ܷ����� ���� ������ �ٿ����� ��ü HUDWidget, �������Ʈ���� �巡�׼����ؾ���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay; // HUDOverlay�� ������ �������� ������� ����, HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass) �̷������� ���
	
};
