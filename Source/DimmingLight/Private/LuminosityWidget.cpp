#include "LuminosityWidget.h"

#include "MainGameMode.h"
#include "MainGameState.h"
#include "Components/Image.h"
#include "Logging/StructuredLog.h"

void ULuminosityWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetIsFocusable(false);

	UWorld* World = GetWorld();
	if (!World) {
		UE_LOGFMT(LogTemp, Error, "ULuminosityWidget Error | NativeConstruct: Failed to get world");
		return;
	}

	GameState = Cast<AMainGameState>(World->GetGameState());
	if (!GameState) {
		UE_LOGFMT(LogTemp, Error, "ULuminosityWidget Error | NativeConstruct: Failed to get game state");
		return;
	}

	GameState->LuminosityChangeDelegate.AddDynamic(this, &ULuminosityWidget::OnLuminosityChange);
	OnLuminosityChange(GameState->GetLightLeftInSeconds() / GameState->GetLightTotalInSeconds());
}

void ULuminosityWidget::OnLuminosityChange(float LuminosityPercentage) {
	// if (!Candle) {
	// 	UE_LOGFMT(LogTemp, Error, "ULuminosityWidget Error | OnLuminosityChange: Failed to get widget image");
	// 	return;
	// }
	// Candle->GetDynamicMaterial()->SetScalarParameterValue("Percentage", LuminosityPercentage);

	if (!Orb) {
		UE_LOGFMT(LogTemp, Error, "ULuminosityWidget Error | OnLuminosityChange: Failed to get widget image");
		return;
	}

	Orb->GetDynamicMaterial()->SetScalarParameterValue("Percentage", LuminosityPercentage);
}
