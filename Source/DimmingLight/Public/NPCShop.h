#pragma once

#include "CoreMinimal.h"
#include "FUpgradeData.h"
#include "NPCInteractable.h"
#include "NPCShop.generated.h"


class ABaseHUD;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpgradeAvailabilityChange, FUpgradeData, NewAvailableUpgrades);

UCLASS()
class DIMMINGLIGHT_API ANPCShop : public ANPCInteractable {
	GENERATED_BODY()

	public:
		ANPCShop();
		virtual void BeginPlay() override;

		// DELEGATES
		FUpgradeAvailabilityChange UpgradeAvailabilityChangeDelegate;

		UFUNCTION(BlueprintCallable, Category="Upgrades")
		FUpgradeData GetAvailableUpgrades();

		// INTERACTABLE
		virtual void Interact_Implementation(AActor* InteractionActor) override;
		virtual void OnUnfocused_Implementation() override;

	private:
		UPROPERTY(EditAnywhere, Category="Upgrades")
		FUpgradeData AvailableUpgrades;

		UPROPERTY(Transient, SkipSerialization)
		ABaseHUD* HUD = nullptr;
};
