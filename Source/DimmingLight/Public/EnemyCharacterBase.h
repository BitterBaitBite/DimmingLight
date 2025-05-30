// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EDamageType.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

class ALightDrop;
enum class EDamageType : uint8;

UCLASS()
class DIMMINGLIGHT_API AEnemyCharacterBase : public ACharacter {
	GENERATED_BODY()

	public:
		AEnemyCharacterBase();
		virtual void BeginPlay() override;

		// CURRENCY REWARD
		UPROPERTY(EditDefaultsOnly, Category="Currency")
		int32 CurrencyReward = 100;

		// DAMAGEABLE
		UFUNCTION(BlueprintCallable, Category="Damageable")
		int32 GetMaxHealth();
		UFUNCTION(BlueprintCallable, Category="Damageable")
		int32 GetHealth();

		UFUNCTION(BlueprintCallable, Category="Damageable")
		virtual float TakeDamage(
			float DamageAmount,
			const struct FDamageEvent& DamageEvent,
			class AController* EventInstigator,
			AActor* DamageCauser
		) override;

		UFUNCTION(BlueprintCallable, Category="Death")
		virtual void Death();
		UFUNCTION(BlueprintCallable, Category="Death")
		virtual void OnDeathMontageEnd();

		// ANIMATIONS
		UPROPERTY(EditDefaultsOnly, Category="Animations")
		TObjectPtr<UAnimMontage> DeathAnimation;

		// EFFECTS
		UPROPERTY(EditDefaultsOnly, Category="Effects")
		TObjectPtr<UParticleSystem> DeathFX;

		// DROPS
		UPROPERTY(EditAnywhere, Category="Drops")
		float LightDropChance = 0.25f;
		UPROPERTY(EditDefaultsOnly, Category="Drops")
		TSubclassOf<ALightDrop> LightDropClass = nullptr;

		// SOCKETS
		UPROPERTY(EditDefaultsOnly, Category="Sockets")
		FName FXRoot_Socket = TEXT("FX_Root");
		UPROPERTY(EditDefaultsOnly, Category="Sockets")
		FName DamageOrigin_Socket = TEXT("FX_Trail_02_R");
		UPROPERTY(EditDefaultsOnly, Category="Sockets")
		FName DamageEnd_Socket = TEXT("FX_Trail_01_R");

		// DAMAGE
		UFUNCTION(BlueprintCallable, Category="Damage")
		float GetDamage();
		UFUNCTION(BlueprintCallable, Category="Damage")
		EDamageType GetDamageType();

	private:
		UPROPERTY(EditAnywhere, Category="Stats")
		float Damage = 20.f;
		UPROPERTY(EditAnywhere, Category="Stats")
		EDamageType DamageType = EDamageType::None;
		UPROPERTY(EditAnywhere, Category="Stats")
		int32 MaxHealth = 100.f;
		UPROPERTY(EditAnywhere, Category="Stats")
		int32 Health = 100.f;
};
