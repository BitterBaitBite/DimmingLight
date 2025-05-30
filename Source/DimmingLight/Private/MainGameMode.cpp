#include "MainGameMode.h"

#include "ELevelType.h"
#include "MainGameInstance.h"
#include "Logging/StructuredLog.h"


AMainGameMode::AMainGameMode() {
	PrimaryActorTick.bCanEverTick = false;
}

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();

	FInputModeGameOnly GameOnly;
	auto* PC = GetWorld()->GetFirstPlayerController();
	PC->SetInputMode(GameOnly);
}

float AMainGameMode::GetLightTotalInSeconds() {
	return LightTotalTime;
}
