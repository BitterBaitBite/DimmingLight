#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EDamageType : uint8 {
	Physical UMETA(DisplayName = "Physical"),
	Magic UMETA(DisplayName = "Magic"),
	None UMETA(DisplayName = "None")
};
