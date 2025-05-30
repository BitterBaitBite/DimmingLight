#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"


UCLASS()
class DIMMINGLIGHT_API AMainMenuHUD : public AHUD {
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;

	private:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess=true))
		TSubclassOf<UUserWidget> MainMenuTemplate = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UUserWidget> MainMenuWidget = nullptr;
};
