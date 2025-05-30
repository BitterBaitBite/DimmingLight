// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "ELevelType.h"
#include "MainGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "UMainMenuWidget Error | NativeConstruct: Game Instance couldn't be loaded");
		return;
	}
	GameInstance->CurrentLevelType = ELevelType::MainMenu;

	if (!PlayButton) {
		UE_LOGFMT(LogTemp, Error, "UMainMenuWidget Error | NativeConstruct: Play button is null");
		return;
	}
	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayButtonClicked);

	if (!ExitButton) {
		UE_LOGFMT(LogTemp, Error, "UMainMenuWidget Error | NativeConstruct: Exit button is null");
		return;
	}
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
}

void UMainMenuWidget::OnPlayButtonClicked() {
	if (!GameInstance) return;

	GameInstance->LoadLevelByType(ELevelType::SafeArea);
}

void UMainMenuWidget::OnExitButtonClicked() {
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
