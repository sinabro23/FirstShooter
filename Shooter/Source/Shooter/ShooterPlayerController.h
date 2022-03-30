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
	TSubclassOf<class UUserWidget> HUDOverlayClass; // 총알 잔량같은 작은 위젯을 붙여넣을 전체 HUDWidget, 블루프린트에서 드래그선택해야함

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay; // HUDOverlay를 만든후 참조값을 들고있을 변수, HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass) 이런식으로 사용
	
};
