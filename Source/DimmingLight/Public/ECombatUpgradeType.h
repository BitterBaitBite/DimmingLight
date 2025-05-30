#pragma once

UENUM(BlueprintType)
enum class ECombatUpgradeType : uint8 {
	MainAttackUpgrade UMETA(DisplayName = "Main Attack Upgrade"),
	SpecialAttackUpgrade UMETA(DisplayName = "Special Attack Upgrade"),
	DodgeUpgrade UMETA(DisplayName = "Dodge Upgrade"),
	Count,
};
