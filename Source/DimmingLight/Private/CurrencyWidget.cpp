#include "CurrencyWidget.h"

#include "MainGameInstance.h"
#include "Components/TextBlock.h"
#include "Logging/StructuredLog.h"


void UCurrencyWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetIsFocusable(false);
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "UCurrencyWidget Error | NativeConstruct: Failed to get game instance");
		return;
	}
	GameInstance->CurrencyChangeDelegate.AddDynamic(this, &UCurrencyWidget::OnCurrencyChange);
	OnCurrencyChange(GameInstance->GetCurrency());
}

void UCurrencyWidget::OnCurrencyChange(int32 NewCurrencyValue) {
	if (!CurrencyText) {
		UE_LOGFMT(LogTemp, Error, "UCurrencyWidget Error | OnCurrencyChange: Currency text is null");
		return;
	}

	CurrencyText->SetText(FText::AsNumber(NewCurrencyValue));
}
