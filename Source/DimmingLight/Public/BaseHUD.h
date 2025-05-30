#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"


UCLASS(Hidden)
class DIMMINGLIGHT_API ABaseHUD : public AHUD {
	GENERATED_BODY()

	public:
		UFUNCTION()
		virtual void BeginPlay() override;

		UFUNCTION(BlueprintCallable)
		void OpenShop();
		UFUNCTION(BlueprintCallable)
		void CloseShop();

	private:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess=true))
		TSubclassOf<UUserWidget> LuminosityWidgetTemplate = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UUserWidget> LuminosityWidget = nullptr;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess=true))
		TSubclassOf<UUserWidget> HealthWidgetTemplate = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UUserWidget> HealthWidget = nullptr;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess=true))
		TSubclassOf<UUserWidget> InteractWidgetTemplate = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UUserWidget> InteractWidget = nullptr;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess=true))
		TSubclassOf<UUserWidget> CurrencyWidgetTemplate = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UUserWidget> CurrencyWidget = nullptr;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess=true))
		TSubclassOf<UUserWidget> ShopWidgetTemplate = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UUserWidget> ShopWidget = nullptr;

		UFUNCTION()
		void OnPlayerDeath();
};
