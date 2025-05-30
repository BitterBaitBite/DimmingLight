#include "MainGameInstance.h"

#include "ELevelType.h"
#include "EStatUpgradeType.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

int32 UMainGameInstance::GetCurrency() {
	return PlayerCurrency;
}

void UMainGameInstance::AddCurrency(int32 AmountToAdd) {
	PlayerCurrency += AmountToAdd;
	CurrencyChangeDelegate.Broadcast(PlayerCurrency);
}

void UMainGameInstance::RemoveCurrency(int32 AmountToRemove) {
	PlayerCurrency -= AmountToRemove;
	CurrencyChangeDelegate.Broadcast(PlayerCurrency);
}

FUpgradeData UMainGameInstance::GetUpgradeData() {
	return FUpgradeData();
}

void UMainGameInstance::SetUpgradeData(FUpgradeData NewUpgradeData) {
	UpgradeData = NewUpgradeData;
}

int32 UMainGameInstance::GetMaxHealthUpgraded(int32 BaseMaxHealth) {
	if (UpgradeData.StatUpgrades.Contains(EStatUpgradeType::HealthUpgrade)) {
		int32 HealthUpgrades = UpgradeData.StatUpgrades[EStatUpgradeType::HealthUpgrade];
		return BaseMaxHealth + HealthUpgrades * HealthUpgradeValue;
	}

	return BaseMaxHealth;
}

float UMainGameInstance::GetBasicAttackDamageUpgraded(float BaseDamage) {
	if (UpgradeData.StatUpgrades.Contains(EStatUpgradeType::DamageUpgrade)) {
		int32 DamageUpgrades = UpgradeData.StatUpgrades[EStatUpgradeType::DamageUpgrade];
		return BaseDamage + DamageUpgrades * BasicAttackDamageUpgradeValue;
	}

	return BaseDamage;
}

float UMainGameInstance::GetSpecialAttackDamageUpgraded(float BaseDamage) {
	if (UpgradeData.StatUpgrades.Contains(EStatUpgradeType::DamageUpgrade)) {
		int32 DamageUpgrades = UpgradeData.StatUpgrades[EStatUpgradeType::DamageUpgrade];
		return BaseDamage + DamageUpgrades * SpecialAttackDamageUpgradeValue;
	}

	return BaseDamage;
}

float UMainGameInstance::GetLightTotalTimeUpgraded(float BaseLightInSeconds) {
	if (UpgradeData.StatUpgrades.Contains(EStatUpgradeType::LuminosityUpgrade)) {
		int32 LuminosityUpgrades = UpgradeData.StatUpgrades[EStatUpgradeType::LuminosityUpgrade];
		return BaseLightInSeconds + LuminosityUpgrades * LightUpgradeInSeconds;
	}

	return BaseLightInSeconds;
}

float UMainGameInstance::GetPhysicalResistanceUpgraded(float BasePhysicalResistance) {
	if (UpgradeData.StatUpgrades.Contains(EStatUpgradeType::PhysicalResistanceUpgrade)) {
		int32 PhysicalResistanceUpgrades = UpgradeData.StatUpgrades[EStatUpgradeType::PhysicalResistanceUpgrade];
		return BasePhysicalResistance + PhysicalResistanceUpgrades * PhysicalResistanceUpgradeValue;
	}

	return BasePhysicalResistance;
}

float UMainGameInstance::GetMagicResistanceUpgraded(float BaseMagicResistance) {
	if (UpgradeData.StatUpgrades.Contains(EStatUpgradeType::PhysicalResistanceUpgrade)) {
		int32 PhysicalResistanceUpgrades = UpgradeData.StatUpgrades[EStatUpgradeType::PhysicalResistanceUpgrade];
		return BaseMagicResistance + PhysicalResistanceUpgrades * PhysicalResistanceUpgradeValue;
	}

	return BaseMagicResistance;
}

void UMainGameInstance::LoadNextLevel() {
	if (CurrentLevelType == ELevelType::Unknown) {
		UE_LOGFMT(LogTemp, Error, "UMainGameInstance Error | LoadNextLevel: The current level type is Unknown");
	}

	switch (CurrentLevelType) {
		case ELevelType::SafeArea:
			UGameplayStatics::OpenLevel(this, CombatArea_LvlName);
			CurrentLevelType = ELevelType::CombatArea;
			break;

		case ELevelType::CombatArea:
			UGameplayStatics::OpenLevel(this, SafeArea_LvlName);
			CurrentLevelType = ELevelType::SafeArea;
			break;

		case ELevelType::EndArea:
			UGameplayStatics::OpenLevel(this, CombatArea_LvlName);
			CurrentLevelType = ELevelType::CombatArea;
			break;

		case ELevelType::MainMenu:
			UGameplayStatics::OpenLevel(this, SafeArea_LvlName);
			CurrentLevelType = ELevelType::SafeArea;
			break;

		default:
			UGameplayStatics::OpenLevel(this, MainMenu_LvlName);
			CurrentLevelType = ELevelType::MainMenu;
			break;
	}

	LevelChangeDelegate.Broadcast(CurrentLevelType);
}

void UMainGameInstance::LoadLevelByType(ELevelType LevelType) {
	switch (LevelType) {
		case ELevelType::SafeArea:
			UGameplayStatics::OpenLevel(this, SafeArea_LvlName);
			CurrentLevelType = ELevelType::SafeArea;
			break;

		case ELevelType::CombatArea:
			UGameplayStatics::OpenLevel(this, CombatArea_LvlName);
			CurrentLevelType = ELevelType::CombatArea;
			break;

		case ELevelType::EndArea:
			UGameplayStatics::OpenLevel(this, EndArea_LvlName);
			CurrentLevelType = ELevelType::EndArea;
			break;

		case ELevelType::MainMenu:
			UGameplayStatics::OpenLevel(this, MainMenu_LvlName);
			CurrentLevelType = ELevelType::MainMenu;
			break;

		default:
			UGameplayStatics::OpenLevel(this, MainMenu_LvlName);
			CurrentLevelType = ELevelType::MainMenu;
			break;
	}

	LevelChangeDelegate.Broadcast(CurrentLevelType);
}

void UMainGameInstance::LoadLevelByName(FName LevelName) {
	UGameplayStatics::OpenLevel(this, LevelName);

	LevelChangeDelegate.Broadcast(CurrentLevelType);
}

void UMainGameInstance::ExitToMainMenu() {
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));

	LevelChangeDelegate.Broadcast(CurrentLevelType);
}
