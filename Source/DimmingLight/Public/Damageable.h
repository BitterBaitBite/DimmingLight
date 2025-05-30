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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		virtual void ReceiveDamage(float RawDamage, EDamageType DamageType) = 0;
};
