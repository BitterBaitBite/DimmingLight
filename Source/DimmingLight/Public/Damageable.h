#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"


enum class EDamageType : uint8;

UINTERFACE()
class UDamageable : public UInterface {
	GENERATED_BODY()
};

class DIMMINGLIGHT_API IDamageable {
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
		void ReceiveDamage(float RawDamage, EDamageType DamageType);
};
