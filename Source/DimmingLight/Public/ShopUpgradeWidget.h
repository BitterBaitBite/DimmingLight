#pragma once

#include "CoreMinimal.h"
#include "EStatUpgradeType.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "ShopUpgradeWidget.generated.h"


class UMainGameInstance;
class UButton;
class UTextBlock;
class UImage;

UCLASS()
class DIMMINGLIGHT_API UShopUpgradeWidget : public UUserWidget, public IUserObjectListEntry {
	GENERATED_BODY()

	public:
		UFUNCTION()
		virtual void NativeConstruct() override;

		// UPGRADE DATA
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Upgrade")
		EStatUpgradeType UpgradeType = EStatUpgradeType::Count;

		// UI ELEMENTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="UpgradeImage"))
		TObjectPtr<UImage> UpgradeImage = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="UpgradeText"))
		TObjectPtr<UTextBlock> UpgradeText = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="UpgradeCostText"))
		TObjectPtr<UTextBlock> UpgradeCostText = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="UpgradeButton"))
		TObjectPtr<UButton> UpgradeButton = nullptr;

		// GAMEPLAY
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UMainGameInstance> GameInstance = nullptr;

		// EVENTS
		UFUNCTION()
		void OnBuyUpgrade();
};
