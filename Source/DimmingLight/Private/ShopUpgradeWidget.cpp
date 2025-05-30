#include "ShopUpgradeWidget.h"

#include "MainGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Logging/StructuredLog.h"

void UShopUpgradeWidget::NativeConstruct() {
	Super::NativeConstruct();

	GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "UShopUpgrade Error | NativeConstruct: Game Instance couldn't be loaded");
		return;
	}

	UpgradeButton->OnClicked.AddDynamic(this, &UShopUpgradeWidget::OnBuyUpgrade);
}

void UShopUpgradeWidget::OnBuyUpgrade() {
	FString UpgradeCostString = UpgradeCostText->GetText().ToString();
	int32 UpgradeCost = FCString::Atoi(*UpgradeCostString);

	if (GameInstance->GetCurrency() < UpgradeCost) {
		return;
	}

	FUpgradeData UpgradeData = GameInstance->GetUpgradeData();
	if (UpgradeData.StatUpgrades.Contains(UpgradeType)) {
		UpgradeData.StatUpgrades[UpgradeType]++;
	}
	else {
		UpgradeData.StatUpgrades.Add(UpgradeType, 1);
	}

	GameInstance->RemoveCurrency(UpgradeCost);
	GameInstance->SetUpgradeData(UpgradeData);

	UpgradeButton->SetVisibility(ESlateVisibility::Collapsed);
	UpgradeCostText->SetText(FText::FromString(TEXT("0")));
}
