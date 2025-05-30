#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class ELevelType : uint8 {
	MainMenu UMETA(DisplayName = "Main Menu"),
	SafeArea UMETA(DisplayName = "Safe Area"),
	CombatArea UMETA(DisplayName = "Combat Area"),
	EndArea UMETA(DisplayName = "End Area"),
	Unknown UMETA(DisplayName = "Unknown")
};
