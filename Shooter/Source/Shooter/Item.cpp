// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"

// Sets default values
AItem::AItem() : 
	ItemName(FString("Default")),
	ItemCount(0),
	ItemRarity(EItemRarity::EIR_Common),
	ItemState(EItemState::EIS_PickUp)
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
		break;
	case EItemState::EIS_PickedUp:
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


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

