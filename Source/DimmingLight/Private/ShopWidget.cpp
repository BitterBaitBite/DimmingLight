#include "ShopWidget.h"

#include "ECombatUpgradeType.h"
#include "EStatUpgradeType.h"
#include "MainGameInstance.h"
#include "NPCShop.h"
#include "ShopUpgradeWidget.h"
#include "Components/StackBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


void UShopWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetIsFocusable(false);

	UGameInstance* CurrentGameInstance = GetGameInstance();

	if (!CurrentGameInstance) {
		UE_LOGFMT(LogTemp, Error, "UShopWidget Error | NativeConstruct: Failed to get current game instance");
		return;
	}

	GameInstance = Cast<UMainGameInstance>(CurrentGameInstance);
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "UShopWidget Error | NativeConstruct: Failed to get the main game instance");
		return;
	}

	// TODO Shop
	FUpgradeData CurrentUpgrades = GameInstance->GetUpgradeData();
	for (int i = 0; i < static_cast<int>(EStatUpgradeType::Count); i++) {
		EStatUpgradeType CurrentUpgradeType = static_cast<EStatUpgradeType>(i);
		switch (CurrentUpgradeType) {
			case EStatUpgradeType::HealthUpgrade:
				HealthUpgradeWidget = CreateWidget<UShopUpgradeWidget>(GetWorld(), UpgradeWidgetTemplate);
				HealthUpgradeWidget->UpgradeText->SetText(FText::FromString(TEXT("Health")));
				HealthUpgradeWidget->UpgradeCostText->SetText(FText::FromString(TEXT("500")));
				HealthUpgradeWidget->UpgradeType = CurrentUpgradeType;
				StatUpgradesBox->AddChild(HealthUpgradeWidget);
				break;
			case EStatUpgradeType::LuminosityUpgrade:
				LuminosityUpgradeWidget = CreateWidget<UShopUpgradeWidget>(
					GetWorld(),
					UpgradeWidgetTemplate
				);
				LuminosityUpgradeWidget->UpgradeText->SetText(FText::FromString(TEXT("Luminosity")));
				LuminosityUpgradeWidget->UpgradeCostText->SetText(FText::FromString(TEXT("500")));
				LuminosityUpgradeWidget->UpgradeType = CurrentUpgradeType;
				StatUpgradesBox->AddChild(LuminosityUpgradeWidget);
				break;
			case EStatUpgradeType::DamageUpgrade:
				DamageUpgradeWidget = CreateWidget<UShopUpgradeWidget>(GetWorld(), UpgradeWidgetTemplate);
				DamageUpgradeWidget->UpgradeText->SetText(FText::FromString(TEXT("Damage")));
				DamageUpgradeWidget->UpgradeCostText->SetText(FText::FromString(TEXT("500")));
				DamageUpgradeWidget->UpgradeType = CurrentUpgradeType;
				StatUpgradesBox->AddChild(DamageUpgradeWidget);
				break;
			case EStatUpgradeType::PhysicalResistanceUpgrade:
				PhysicResistanceUpgradeWidget = CreateWidget<UShopUpgradeWidget>(
					GetWorld(),
					UpgradeWidgetTemplate
				);
				PhysicResistanceUpgradeWidget->UpgradeText->SetText(FText::FromString(TEXT("Physical Resistance")));
				PhysicResistanceUpgradeWidget->UpgradeCostText->SetText(FText::FromString(TEXT("500")));
				PhysicResistanceUpgradeWidget->UpgradeType = CurrentUpgradeType;
				StatUpgradesBox->AddChild(PhysicResistanceUpgradeWidget);
				break;
			case EStatUpgradeType::MagicResistanceUpgrade:
				MagicResistanceUpgradeWidget = CreateWidget<UShopUpgradeWidget>(
					GetWorld(),
					UpgradeWidgetTemplate
				);
				MagicResistanceUpgradeWidget->UpgradeText->SetText(FText::FromString(TEXT("Magic Resistance")));
				MagicResistanceUpgradeWidget->UpgradeCostText->SetText(FText::FromString(TEXT("500")));
				MagicResistanceUpgradeWidget->UpgradeType = CurrentUpgradeType;
				StatUpgradesBox->AddChild(MagicResistanceUpgradeWidget);
				break;
			default:
				break;
		}
	}

	// SHOP NPCs
	TArray<AActor*> ShopActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPCShop::StaticClass(), ShopActors);
	for (AActor* ShopActor : ShopActors) {
		ANPCShop* ShopNPC = Cast<ANPCShop>(ShopActor);
		if (!ShopNPC) {
			UE_LOGFMT(LogTemp, Error, "UShopWidget Error | NativeConstruct: Failed to cast actor to NPC shop");
			continue;
		}

		ShopNPCs.AddUnique(ShopNPC);
		ShopNPC->UpgradeAvailabilityChangeDelegate.AddDynamic(this, &UShopWidget::OnUpgradeAvailabilityChange);
	}
}

void UShopWidget::OnUpgradeAvailabilityChange(FUpgradeData AvailableUpgrades) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Upgrade Availability Change")));
}
