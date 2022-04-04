// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveVector.h"

// Sets default values
AItem::AItem() :
	ItemName(FString("Default")),
	ItemCount(0),
	ItemRarity(EItemRarity::EIR_Common),
	ItemState(EItemState::EIS_PickUp),
	ZCurveTime(0.7f),
	ItemInterpStartLocation(FVector(0.f)),
	CameraTargetLocation(FVector(0.f)),
	bInterping(false),
	ItemInterpX(0.f),
	ItemInterpY(0.f),
	InterpInitialYawOffset(0.f),
	ItemType(EItemType::EIT_MAX),
	InterpLocIndex(0),
	MaterialIndex(0),
	bCanChangeCustomDepth(true),
	// 이해안된것 머테리얼 파라미터스
	GlowAmount(150.f),
	FresnelExponent(3.f),
	FresnelReflectFraction(4.f),
	PulseCurveTime(5.f),
	SlotIndex(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollsionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBox"));
	CollsionBox->SetupAttachment(ItemMesh);
	CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 모든 채널로부터의 충돌을 다 거부하고
	CollsionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); // ECC_Visibility채널로만 충돌되게

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget")); // 스크린모드로 하는것은 블루프린트에서 했음
	PickupWidget->SetupAttachment(GetRootComponent()); 

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent()); // Radius는 블루프린트에서 설정했음

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false); // 기본적으로 위젯 안보이게
	}
	
	SetActiveStars(); // 아이템 희귀도에 따른 별활성화 // 위젯 블루프린트에서 별 색깔로 바인드해서 알파값 바꿔서 꺼버림
	
	// AreaSphere 오버랩관련
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	// 아이템 상태에 따른 아이템 설정들(콜리전같은것)
	SetItemProperties(ItemState);

	// 커스텀뎁스 불가능하게
	InitializeCustomDepth();

	StartPulseTimer();
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if(ShooterCharacter)
		{
			ShooterCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OtherActor);
		if (ShooterCharacter)
		{
			ShooterCharacter->IncrementOverlappedItemCount(-1);
		}
	}
}

void AItem::SetActiveStars()
{
	// 0번은 쓰지 않음
	for (int32 i = 0; i <= 5; i++)
	{
		ActiveStars.Add(false);
	}

	switch (ItemRarity)
	{
	case EItemRarity::EIR_Damaged:
		ActiveStars[1] = true;
		break;
	case EItemRarity::EIR_Common:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		break;
	case EItemRarity::EIR_UnCommon:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		break;
	case EItemRarity::EIR_Rare:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		break;
	case EItemRarity::EIR_Legendary:
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		ActiveStars[5] = true;
		break;
	default:
		break;
	}
}

void AItem::SetItemProperties(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_PickUp:
		// 메쉬관련 설정
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// AreaSphere(위젯뜨는범위 콜리전)
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		//CollsionBox(라인트레이싱 당하는 콜리전)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EItemState::EIS_EquipInterping:
		PickupWidget->SetVisibility(false);
		
		// 메쉬관련 설정
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// AreaSphere(위젯뜨는범위 콜리전) : Interp될때 필요없음
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//CollsionBox(라인트레이싱 당하는 콜리전)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	case EItemState::EIS_PickedUp:
		PickupWidget->SetVisibility(false);

		// 메쉬관련 설정
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// AreaSphere(위젯뜨는범위 콜리전) : Interp될때 필요없음
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//CollsionBox(라인트레이싱 당하는 콜리전)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Equipped: // 손에 들고 있을때

		PickupWidget->SetVisibility(false);

		// 메쉬관련 설정
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// AreaSphere(위젯뜨는범위 콜리전) : 손에들고있을때 필요없음
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//CollsionBox(라인트레이싱 당하는 콜리전)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Falling:

		// 메쉬 바닥으로 떨어지게 하기 위해
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		// AreaSphere(위젯뜨는범위 콜리전) : 떨어질때 필요없음
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//CollsionBox(라인트레이싱 당하는 콜리전) : 떨어질때 필요없음
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	case EItemState::EIS_Max:
		break;
	default:
		break;
	}
}

void AItem::FinishInterping() // interp타이머 끝나면 호출될 함수
{
	bInterping = false;
	if (Character)
	{
		//Subtract 1 from the Item Count of the interp location struct
		Character->IncrementInterpLocItemCount(InterpLocIndex, -1);
		Character->GetPickupItem(this);
	}

	SetActorScale3D(FVector(1.f));

	DisableGlowMaterial();
	bCanChangeCustomDepth = true;
	DisableCustomDepth();
}

void AItem::ItemInterp(float DeltaTime)
{
	if (!bInterping)
		return;

	if (Character && ItemZCurve ) //ItemZCurve는 블루프린트에서 세팅함
	{
		// ElapseTime : ItemInterpTimer 시작하고 경과시간
		const float ElapseTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
		// 경과시간에 따른 UCurveFloat에서 받아온 값
		const float CurveValue = ItemZCurve->GetFloatValue(ElapseTime);
		// 커브가 시작될때 아이템의 첫 위치를 구함
		FVector ItemLocation = ItemInterpStartLocation;
		// 카메라 앞에 멈출 로케이션
		const FVector CameraInterpLocation = GetInterpLocation();
		// 아이템에서 카메라 앞에 멈출 로케이션 까지의 벡터 (x,y는 0)
		const FVector ItemToCamera = FVector(0.f, 0.f, (CameraInterpLocation - ItemLocation).Z);
		// CurveValue와 곱해질 값.
		const float DeltaZ = ItemToCamera.Size();

		const FVector CurrentLocation = GetActorLocation();
		const float InterpXValue = FMath::FInterpTo(CurrentLocation.X, CameraInterpLocation.X, DeltaTime, 30.f);
		const float InterpYValue = FMath::FInterpTo(CurrentLocation.Y, CameraInterpLocation.Y, DeltaTime, 30.f);

		ItemLocation.X = InterpXValue;
		ItemLocation.Y = InterpYValue;

		ItemLocation.Z += CurveValue * DeltaZ;
		SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);

		const FRotator CameraRotation = Character->GetFollowCamera()->GetComponentRotation();
		FRotator ItemRotation = FRotator(0.f, CameraRotation.Yaw + InterpInitialYawOffset, 0.f);
		SetActorRotation(ItemRotation, ETeleportType::TeleportPhysics);

		if (ItemScaleCurve)
		{
			const float ScaleCurveValue = ItemScaleCurve->GetFloatValue(ElapseTime);
			SetActorScale3D(FVector(ScaleCurveValue, ScaleCurveValue, ScaleCurveValue));
		}
		
	}


}

FVector AItem::GetInterpLocation()
{
	if (Character == nullptr)
		return FVector(0.f);

	switch (ItemType)
	{
	case EItemType::EIT_AMMO:
		return Character->GetInterpLocation(InterpLocIndex).SceneComponent->GetComponentLocation();
		break;
	case EItemType::EIT_Weapon:
		return Character->GetInterpLocation(0).SceneComponent->GetComponentLocation();
		break;
	default:
		break;
	}

	return FVector();
}

void AItem::PlayPickupSound()
{
	if(Character)
	{
		if (Character->ShouldPlayPickupSound())
		{
			Character->StartPickupSoundTimer();
			if (PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
		}
	}
}

void AItem::EnableCustomDepth()
{
	if (bCanChangeCustomDepth)
	{
		ItemMesh->SetRenderCustomDepth(true);
	}
}

void AItem::DisableCustomDepth()
{
	if (bCanChangeCustomDepth)
	{
		ItemMesh->SetRenderCustomDepth(false);
	}
}

void AItem::InitializeCustomDepth()
{
	DisableCustomDepth();
}

void AItem::OnConstruction(const FTransform& Transform)
{
	if (MaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance, this);
		ItemMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);
	}

	EnableGlowMaterial();
}

void AItem::UpdatePulse()
{
	float ElapsedTime = 0.f;
	FVector CurveValue = FVector();

	switch (ItemState)
	{
	case EItemState::EIS_PickUp:
		if (PulseCurve)
		{
			ElapsedTime = GetWorldTimerManager().GetTimerElapsed(PulseTimer);
			CurveValue = PulseCurve->GetVectorValue(ElapsedTime);
		}
		break;
	case EItemState::EIS_EquipInterping:
		if (InterpPulseCurve)
		{
			ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
			CurveValue = InterpPulseCurve->GetVectorValue(ElapsedTime);
		}
		break;
	default:
		break;
	}

	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowAmount"), CurveValue.X * GlowAmount);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("FresnelExponent"), CurveValue.Y * FresnelExponent);
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("FresnelReflectFraction"), CurveValue.Z * FresnelReflectFraction);
	}
}

void AItem::EnableGlowMaterial()
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowBlendAlpha"), 0);
	}
}

void AItem::DisableGlowMaterial()
{
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("GlowBlendAlpha"), 1);
	}
}

void AItem::PlayEquipSound()
{
	if (Character)
	{
		if (Character->ShouldPlayEquipSound())
		{
			Character->StartEquipSoundTimer();
			if (EquipSound)
			{
				UGameplayStatics::PlaySound2D(this, EquipSound);
			}
		}
	}
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemInterp(DeltaTime);
	UpdatePulse();

}

void AItem::ResetPulseTimer()
{
	StartPulseTimer();
}

void AItem::StartPulseTimer()
{
	if (ItemState == EItemState::EIS_PickUp)
	{
		GetWorldTimerManager().SetTimer(PulseTimer, this, &AItem::ResetPulseTimer, PulseCurveTime);
	}
}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

void AItem::StartItemCurve(AShooterCharacter* Char)
{
	Character = Char;

	// Get Array index in InterpLocations with the lowest item count
	InterpLocIndex = Character->GetInterpLocationIndex();
	// Add 1 to the Item Count for this interp location struct
	Character->IncrementInterpLocItemCount(InterpLocIndex, 1);

	PlayPickupSound();

	ItemInterpStartLocation = GetActorLocation();
	bInterping = true;
	SetItemState(EItemState::EIS_EquipInterping);
	GetWorldTimerManager().ClearTimer(PulseTimer);

	GetWorldTimerManager().SetTimer(ItemInterpTimer, this, &AItem::FinishInterping, ZCurveTime);

	const float CameraRotationYaw = Character->GetFollowCamera()->GetComponentRotation().Yaw;
	const float ItemRotationYaw = GetActorRotation().Yaw;

	InterpInitialYawOffset = ItemRotationYaw - CameraRotationYaw;

	bCanChangeCustomDepth = false;
}

