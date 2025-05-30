#include "DimmingLight/Public/HealthWidget.h"

#include "MainCharacter.h"
#include "Components/Image.h"
#include "Logging/StructuredLog.h"

class AMainCharacter;

void UHealthWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetIsFocusable(false);

	APawn* OwnerPawn = GetOwningPlayerPawn();

	if (!OwnerPawn) {
		UE_LOGFMT(LogTemp, Error, "UHealthWidget Error | NativeConstruct: Failed to get owner pawn");
		return;
	}

	OwnerCharacter = Cast<AMainCharacter>(OwnerPawn);

	if (!OwnerCharacter) {
		UE_LOGFMT(LogTemp, Error, "UHealthBar Error | NativeConstruct: Failed to get the owner character");
		return;
	}

	OwnerCharacter->HealthChangeDelegate.AddDynamic(this, &UHealthWidget::OnHealthChange);
}

void UHealthWidget::OnHealthChange(int32 NewHealth) {
	if (!OwnerCharacter) {
		UE_LOGFMT(LogTemp, Error, "UHealthWidget Error | OnHealthChange: Owner character is null");
		return;
	}

	if (!Orb) {
		UE_LOGFMT(LogTemp, Error, "UHealthWidget Error | OnHealthChange: Orb is null");
		return;
	}

	int32 CurrentHealth = OwnerCharacter->GetHealth();
	int32 MaxHealth = OwnerCharacter->GetMaxHealth();

	if (CurrentHealth != NewHealth) {
		UE_LOGFMT(
			LogTemp,
			Warning,
			"UHealthWidget Warning | OnHealthChange: Current health is not equal to new health"
		);
		CurrentHealth = NewHealth;
	}

	if (MaxHealth <= 0) {
		UE_LOGFMT(LogTemp, Warning, "UHealthWidget Warning | OnHealthChange: Max health is <= 0");
		Orb->GetDynamicMaterial()->SetScalarParameterValue("Percentage", 0.f);
	}
	else {
		float Percentage = 1.f * CurrentHealth / MaxHealth;
		Orb->GetDynamicMaterial()->SetScalarParameterValue("Percentage", Percentage);
	}
}
