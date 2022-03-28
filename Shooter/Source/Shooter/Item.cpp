// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	CollsionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBox"));
	CollsionBox->SetupAttachment(ItemMesh);
	CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ��� ä�ηκ����� �浹�� �� �ź��ϰ�
	CollsionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); // ECC_Visibilityä�ηθ� �浹�ǰ�

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget")); // ��ũ������ �ϴ°��� �������Ʈ���� ����
	PickupWidget->SetupAttachment(GetRootComponent()); 
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	PickupWidget->SetVisibility(false); // �⺻������ ���� �Ⱥ��̰�
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

