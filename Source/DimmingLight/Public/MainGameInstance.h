#pragma once

#include "CoreMinimal.h"
#include "FUpgradeData.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"


class AMainWorldSettings;
enum class ELevelType : uint8;
class ALevelInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrencyChange, int32, NewCurrencyValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelChange, ELevelType, NewLevelType);

UCLASS()
class DIMMINGLIGHT_API UMainGameInstance : public UGameInstance {
	GENERATED_BODY()

	public:
		FCurrencyChange CurrencyChangeDelegate;
		FLevelChange LevelChangeDelegate;

		// CURRENCY
		UFUNCTION(BlueprintCallable, Category="Currency")
		int32 GetCurrency();
		UFUNCTION(BlueprintCallable, Category="Currency")
		void AddCurrency(int32 AmountToAdd);
		UFUNCTION(BlueprintCallable, Category="Currency")
		void RemoveCurrency(int32 AmountToRemove);

		// UPGRADES DATA
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		FUpgradeData GetUpgradeData();
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		void SetUpgradeData(FUpgradeData NewUpgradeData);

		// STAT UPGRADES
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		int32 GetMaxHealthUpgraded(int32 BaseMaxHealth);
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		float GetBasicAttackDamageUpgraded(float BaseDamage);
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		float GetSpecialAttackDamageUpgraded(float BaseDamage);
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		float GetLightTotalTimeUpgraded(float BaseLuminosity);
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		float GetPhysicalResistanceUpgraded(float BasePhysicalResistance);
		UFUNCTION(BlueprintCallable, Category="Upgrades")
		float GetMagicResistanceUpgraded(float BaseMagicResistance);

		// LEVEL MANAGING
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
		FName MainMenu_LvlName = FName("MainMenu");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
		FName SafeArea_LvlName = FName("SafeArea");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
		FName CombatArea_LvlName = FName("CombatArea");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
		FName EndArea_LvlName = FName("EndArea");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
		ELevelType CurrentLevelType;

		UFUNCTION(BlueprintCallable, Category="Level")
		void LoadNextLevel();
		UFUNCTION(BlueprintCallable, Category="Level")
		void LoadLevelByType(ELevelType LevelType);
		UFUNCTION(BlueprintCallable, Category="Level")
		void LoadLevelByName(FName LevelName);

		UFUNCTION(BlueprintCallable, Category="Level")
		void ExitToMainMenu();

	private:
		UPROPERTY(EditAnywhere, Category="Currency")
		int32 PlayerCurrency = 0;

		UPROPERTY(EditAnywhere, Category="Upgrades")
		FUpgradeData UpgradeData;

		UPROPERTY(EditAnywhere, Category="Upgrade Values")
		int32 HealthUpgradeValue = 20;
		UPROPERTY(EditAnywhere, Category="Upgrade Values")
		float BasicAttackDamageUpgradeValue = 5.f;
		UPROPERTY(EditAnywhere, Category="Upgrade Values")
		float SpecialAttackDamageUpgradeValue = 20.f;
		UPROPERTY(EditAnywhere, Category="Upgrade Values")
		float LightUpgradeInSeconds = 30.f;
		UPROPERTY(EditAnywhere, Category="Upgrade Values")
		float PhysicalResistanceUpgradeValue = 20.f;
		UPROPERTY(EditAnywhere, Category="Upgrade Values")
		float MagicResistanceDamageUpgradeValue = 20.f;
};
