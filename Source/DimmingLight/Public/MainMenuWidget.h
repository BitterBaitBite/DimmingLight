#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


class UMainGameInstance;
class UButton;

UCLASS()
class DIMMINGLIGHT_API UMainMenuWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;

		UPROPERTY(meta=(BindWidget="PlayButton"))
		TObjectPtr<UButton> PlayButton;
		UPROPERTY(meta=(BindWidget="ExitButton"))
		TObjectPtr<UButton> ExitButton;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UMainGameInstance> GameInstance;

		UFUNCTION(BlueprintCallable)
		void OnPlayButtonClicked();
		UFUNCTION(BlueprintCallable)
		void OnExitButtonClicked();
};
