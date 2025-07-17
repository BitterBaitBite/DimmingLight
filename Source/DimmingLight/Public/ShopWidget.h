#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"


class UButton;
class UScrollBox;
class UShopUpgradeWidget;
class UTextBlock;
struct FUpgradeData;
class ANPCShop;
class UMainGameInstance;
class UImage;
class UStackBox;
enum class EStatUpgradeType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopQuit);

UCLASS()
class DIMMINGLIGHT_API UShopWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		UFUNCTION()
		virtual void NativeConstruct() override;

		// DELEGATES
		UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnShopQuit OnShopQuitDelegate;

		// LISTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Upgrade Stacks"))
		TObjectPtr<UScrollBox> StatUpgradesBox = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Upgrade Stacks"))
		TObjectPtr<UScrollBox> CombatUpgradesBox = nullptr;

		// DESCRIPTION
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="DescriptionText"))
		TObjectPtr<UTextBlock> DescriptionText = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="CurrencyText"))
		TObjectPtr<UTextBlock> CurrencyText = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="QuitButton"))
		TObjectPtr<UButton> QuitButton = nullptr;

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

		UFUNCTION()
		void OnCurrencyChange(int32 NewCurrencyValue);

		UFUNCTION()
		void OnQuit();
};
