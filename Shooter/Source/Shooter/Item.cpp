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
	// ���ؾȵȰ� ���׸��� �Ķ���ͽ�
	GlowAmount(150.f),
	FresnelExponent(3.f),
	FresnelReflectFraction(4.f),
	PulseCurveTime(5.f),
	SlotIndex(0),
	bCharacterInvetoryFull(false)
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

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent()); // Radius�� �������Ʈ���� ��������

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false); // �⺻������ ���� �Ⱥ��̰�
	}
	
	SetActiveStars(); // ������ ��͵��� ���� ��Ȱ��ȭ // ���� �������Ʈ���� �� ����� ���ε��ؼ� ���İ� �ٲ㼭 ������
	
	// AreaSphere ����������
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	// ������ ���¿� ���� ������ ������(�ݸ���������)
	SetItemProperties(ItemState);

	// Ŀ���ҵ��� �Ұ����ϰ�
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
			ShooterCharacter->UnHighlightInventorySlot();
		}
	}
}

void AItem::SetActiveStars()
{
	// 0���� ���� ����
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
		// �޽����� ����
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// AreaSphere(�����ߴ¹��� �ݸ���)
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		//CollsionBox(����Ʈ���̽� ���ϴ� �ݸ���)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EItemState::EIS_EquipInterping:
		PickupWidget->SetVisibility(false);
		
		// �޽����� ����
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// AreaSphere(�����ߴ¹��� �ݸ���) : Interp�ɶ� �ʿ����
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//CollsionBox(����Ʈ���̽� ���ϴ� �ݸ���)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	case EItemState::EIS_PickedUp:
		PickupWidget->SetVisibility(false);

		// �޽����� ����
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(false);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// AreaSphere(�����ߴ¹��� �ݸ���) : Interp�ɶ� �ʿ����
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//CollsionBox(����Ʈ���̽� ���ϴ� �ݸ���)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Equipped: // �տ� ��� ������

		PickupWidget->SetVisibility(false);

		// �޽����� ����
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// AreaSphere(�����ߴ¹��� �ݸ���) : �տ���������� �ʿ����
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//CollsionBox(����Ʈ���̽� ���ϴ� �ݸ���)
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Falling:

		// �޽� �ٴ����� �������� �ϱ� ����
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		// AreaSphere(�����ߴ¹��� �ݸ���) : �������� �ʿ����
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//CollsionBox(����Ʈ���̽� ���ϴ� �ݸ���) : �������� �ʿ����
		CollsionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollsionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	case EItemState::EIS_Max:
		break;
	default:
		break;
	}
}

void AItem::FinishInterping() // interpŸ�̸� ������ ȣ��� �Լ�
{
	bInterping = false;
	if (Character)
	{
		//Subtract 1 from the Item Count of the interp location struct
		Character->IncrementInterpLocItemCount(InterpLocIndex, -1);
		Character->GetPickupItem(this);

		Character->UnHighlightInventorySlot();
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

	if (Character && ItemZCurve ) //ItemZCurve�� �������Ʈ���� ������
	{
		// ElapseTime : ItemInterpTimer �����ϰ� ����ð�
		const float ElapseTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
		// ����ð��� ���� UCurveFloat���� �޾ƿ� ��
		const float CurveValue = ItemZCurve->GetFloatValue(ElapseTime);
		// Ŀ�갡 ���۵ɶ� �������� ù ��ġ�� ����
		FVector ItemLocation = ItemInterpStartLocation;
		// ī�޶� �տ� ���� �����̼�
		const FVector CameraInterpLocation = GetInterpLocation();
		// �����ۿ��� ī�޶� �տ� ���� �����̼� ������ ���� (x,y�� 0)
		const FVector ItemToCamera = FVector(0.f, 0.f, (CameraInterpLocation - ItemLocation).Z);
		// CurveValue�� ������ ��.
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

void AItem::PlayPickupSound(bool bForcePlaySound)
{
	if(Character)
	{
		if (bForcePlaySound)
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySound2D(this, PickupSound);
			}
		}

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



	// ���������̺� �ε�
	FString RarityTablePath = TEXT("DataTable'/Game/_Game/DataTable/ItemRarityDataTable.ItemRarityDataTable'");
	UDataTable* RarityTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RarityTablePath));
	if (RarityTableObject)
	{
		FItemRarityTable* RarityRow = nullptr;
		switch (ItemRarity)
		{
		case EItemRarity::EIR_Damaged:
			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Damaged"), TEXT(""));
			break;
		case EItemRarity::EIR_Common:
			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Common"), TEXT(""));
			break;
		case EItemRarity::EIR_UnCommon:
			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("UnCommon"), TEXT(""));
			break;
		case EItemRarity::EIR_Rare:
			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Rare"), TEXT(""));
			break;
		case EItemRarity::EIR_Legendary:
			RarityRow = RarityTableObject->FindRow<FItemRarityTable>(FName("Legendary"), TEXT(""));
			break;
		}

		if (RarityRow)
		{
			GlowColor = RarityRow->GlowColor;
			LightColor = RarityRow->LightColor;
			DarkColor = RarityRow->DarkColor;
			NumberOfStars = RarityRow->NumberOfStars;
			IconBackground = RarityRow->IconBackground;
			if (GetItemMesh())
			{
				GetItemMesh()->SetCustomDepthStencilValue(RarityRow->CustomDepthStencil);
			}
		}
	}

	if (MaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance, this);
		DynamicMaterialInstance->SetVectorParameterValue(TEXT("FresnelColor"), GlowColor);
		ItemMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);

		EnableGlowMaterial();
	}
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

void AItem::PlayEquipSound(bool bForcePlaySound)
{
	if (Character)
	{
		if (bForcePlaySound)
		{
			if (EquipSound)
			{
				UGameplayStatics::PlaySound2D(this, EquipSound);
			}
		}
		else if (Character->ShouldPlayEquipSound())
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

void AItem::StartItemCurve(AShooterCharacter* Char, bool bForcePlaySound)
{
	Character = Char;

	// Get Array index in InterpLocations with the lowest item count
	InterpLocIndex = Character->GetInterpLocationIndex();
	// Add 1 to the Item Count for this interp location struct
	Character->IncrementInterpLocItemCount(InterpLocIndex, 1);

	PlayPickupSound(bForcePlaySound);

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

