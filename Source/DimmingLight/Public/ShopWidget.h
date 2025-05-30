#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"


class UShopUpgradeWidget;
class UTextBlock;
struct FUpgradeData;
class ANPCShop;
class UMainGameInstance;
class UImage;
class UStackBox;
enum class EStatUpgradeType : uint8;

UCLASS()
class DIMMINGLIGHT_API UShopWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;

		// LISTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Upgrade Stacks"))
		TObjectPtr<UStackBox> StatUpgradesBox = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Upgrade Stacks"))
		TObjectPtr<UStackBox> CombatUpgradesBox = nullptr;

		// DESCRIPTION
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="DescriptionText"))
		TObjectPtr<UTextBlock> DescriptionText = nullptr;

		// UPGRADE WIDGETS
		// UPGRADE WIDGETS
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Upgrades")
		TSubclassOf<UUserWidget> UpgradeWidgetTemplate = nullptr;

		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> HealthUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> DamageUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> PhysicResistanceUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> MagicResistanceUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> LuminosityUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> MainAttackUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> SpecialAttackUpgradeWidget = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UShopUpgradeWidget> DodgeUpgradeWidget = nullptr;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UMainGameInstance> GameInstance = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TArray<TObjectPtr<ANPCShop>> ShopNPCs = {};

		UFUNCTION()
		void OnUpgradeAvailabilityChange(FUpgradeData AvailableUpgrades);
};
