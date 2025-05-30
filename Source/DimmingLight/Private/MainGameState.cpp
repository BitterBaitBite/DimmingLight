#include "MainGameState.h"

#include "ELevelType.h"
#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

AMainGameState::AMainGameState() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameState::BeginPlay() {
	Super::BeginPlay();

	GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) {
		UE_LOGFMT(
			LogTemp,
			Error,
			"AMainGameState Error | BeginPlay: Could not find Game Mode"
		);
		return;
	}

	GameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance) {
		UE_LOGFMT(
			LogTemp,
			Error,
			"AMainGameState Error | BeginPlay: Could not find Game Instance"
		);
		return;
	}
	GameInstance->LevelChangeDelegate.AddDynamic(this, &AMainGameState::OnLevelChange);
	OnLevelChange(GameInstance->CurrentLevelType);

	LightTotalTimeInitial = GameMode->GetLightTotalInSeconds();
	LightTotalTimeUpgraded = GameInstance->GetLightTotalTimeUpgraded(LightTotalTimeInitial);
	LightLeftTime = LightTotalTimeUpgraded;

	if (GameInstance->CurrentLevelType == ELevelType::CombatArea) {
		bUpdateLightTime = true;
	}
	else {
		bUpdateLightTime = false;
	}

	LuminosityChangeDelegate.Broadcast(LightLeftTime / LightTotalTimeUpgraded);
}

void AMainGameState::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateLightTime(DeltaTime);
}

float AMainGameState::GetLightLeftInSeconds() {
	return LightLeftTime;
}

float AMainGameState::GetLightTotalInSeconds() {
	return LightTotalTimeUpgraded;
}

void AMainGameState::AddLightInSeconds(float LightInSeconds) {
	LightLeftTime = FMath::Clamp(LightLeftTime + LightInSeconds, 0, LightTotalTimeUpgraded);
}

void AMainGameState::ResetLightTotal() {
	LightTotalTimeInitial = GameMode->GetLightTotalInSeconds();
	LightTotalTimeUpgraded = GameInstance->GetLightTotalTimeUpgraded(LightTotalTimeInitial);
	LightLeftTime = LightTotalTimeUpgraded;
}

void AMainGameState::UpdateLightTime(float DeltaTime) {
	if (LightLeftTime > 0) {
		LightLeftTime = FMath::Clamp(LightLeftTime - DeltaTime, 0, LightTotalTimeUpgraded);
	}

	LuminosityChangeDelegate.Broadcast(LightLeftTime / LightTotalTimeUpgraded);
}

void AMainGameState::OnLevelChange(ELevelType NewLevelType) {
	if (NewLevelType == ELevelType::CombatArea) {
		bUpdateLightTime = true;
	}
	else {
		bUpdateLightTime = false;
	}

	ResetLightTotal();
}
