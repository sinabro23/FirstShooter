#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8  // 총알 분류에 따른 총알량을 맵으로 들고 있기위한 이넘
{
	EAT_9mm UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "Assault Rifle"),

	EAT_MAX UMETA(DisplayName = "Default MAX")
};