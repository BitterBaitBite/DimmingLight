#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LuminosityWidget.generated.h"


class AMainGameState;
class UImage;

UCLASS()
class DIMMINGLIGHT_API ULuminosityWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Frame"))
		TObjectPtr<UImage> Frame;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Glass"))
		TObjectPtr<UImage> Glass;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Orb"))
		TObjectPtr<UImage> Orb;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AMainGameState> GameState = nullptr;

		UFUNCTION()
		void OnLuminosityChange(float LuminosityPercentage);
};
