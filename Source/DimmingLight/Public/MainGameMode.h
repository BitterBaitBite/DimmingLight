#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"


UCLASS(Hidden)
class DIMMINGLIGHT_API AMainGameMode : public AGameModeBase {
	GENERATED_BODY()

	public:
		AMainGameMode();
		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, NotBlueprintable, Category="Sound")
		USoundBase* AmbientSound = nullptr;
		UFUNCTION(BlueprintCallable)
		float GetLightTotalInSeconds();

	private:
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Luminosity")
		float LightTotalTime = 0.f;
};
