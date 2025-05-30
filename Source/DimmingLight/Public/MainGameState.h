#pragma once

#include "CoreMinimal.h"
#include "ELevelType.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"


class AMainGameMode;
class UMainGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLuminosityChange, float, LuminosityPercentage);

UCLASS()
class DIMMINGLIGHT_API AMainGameState : public AGameStateBase {
	GENERATED_BODY()

	public:
		AMainGameState();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UPROPERTY(BlueprintAssignable, Category="Luminosity")
		FOnLuminosityChange LuminosityChangeDelegate;

		UFUNCTION(BlueprintCallable)
		float GetLightLeftInSeconds();

		UFUNCTION(BlueprintCallable)
		float GetLightTotalInSeconds();

		UFUNCTION(BlueprintCallable)
		void AddLightInSeconds(float LightInSeconds);

		UFUNCTION(BlueprintCallable)
		void ResetLightTotal();

	private:
		bool bUpdateLightTime = false;
		void UpdateLightTime(float DeltaTime);

		UPROPERTY(VisibleAnywhere, NotBlueprintable, Category="Luminosity")
		float LightLeftTime = 0.f;

		UPROPERTY(VisibleAnywhere, NotBlueprintable, Category="Luminosity")
		float LightTotalTimeInitial = 180.f;
		UPROPERTY(VisibleAnywhere, NotBlueprintable, Category="Luminosity")
		float LightTotalTimeUpgraded = 180.f;

		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AMainGameMode> GameMode;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UMainGameInstance> GameInstance;

		UFUNCTION()
		void OnLevelChange(ELevelType NewLevelType);
};
