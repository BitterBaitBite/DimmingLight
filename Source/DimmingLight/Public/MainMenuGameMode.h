#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"


UCLASS()
class DIMMINGLIGHT_API AMainMenuGameMode : public AGameModeBase {
	GENERATED_BODY()

	public:
		AMainMenuGameMode();
		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, NotBlueprintable, Category="Sound")
		USoundBase* AmbientSound = nullptr;
};
