#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8  // �Ѿ� �з��� ���� �Ѿ˷��� ������ ��� �ֱ����� �̳�
{
	EAT_9mm UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "Assault Rifle"),

	EAT_MAX UMETA(DisplayName = "Default MAX")
};