#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EStatUpgradeType : uint8 {
	HealthUpgrade UMETA(DisplayName = "Health Upgrade"),
	DamageUpgrade UMETA(DisplayName = "Damage Upgrade"),
	LuminosityUpgrade UMETA(DisplayName = "Luminosity Upgrade"),
	PhysicalResistanceUpgrade UMETA(DisplayName = "Physical Resistance Upgrade"),
	MagicResistanceUpgrade UMETA(DisplayName = "Magic Resistance Upgrade"),
	Count
};
