#include "MainMenuGameMode.h"

#include "ELevelType.h"
#include "MainGameInstance.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
AMainMenuGameMode::AMainMenuGameMode() {}

void AMainMenuGameMode::BeginPlay() {
	Super::BeginPlay();

	FInputModeUIOnly UIMode;
	auto* PC = GetWorld()->GetFirstPlayerController();
	PC->SetInputMode(UIMode);
	PC->bShowMouseCursor = true;

	// Initialize level type
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "AMainMenuGameMode Error | BeginPlay: Could not find Game Instance");
		return;
	}
	GameInstance->CurrentLevelType = ELevelType::MainMenu;

	// Sound
	UGameplayStatics::PlaySound2D(GetWorld(), AmbientSound);
}
