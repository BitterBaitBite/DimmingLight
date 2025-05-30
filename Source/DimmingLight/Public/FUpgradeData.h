#pragma once

#include "CoreMinimal.h"
#include "FUpgradeData.generated.h"

enum class ECombatUpgradeType : uint8;
enum class EStatUpgradeType : uint8;

USTRUCT(BlueprintType)
struct DIMMINGLIGHT_API FUpgradeData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EStatUpgradeType, int32> StatUpgrades = {};

	UPROPERTY(EditAnywhere)
	TArray<ECombatUpgradeType> CombatUpgrades = {};
};
