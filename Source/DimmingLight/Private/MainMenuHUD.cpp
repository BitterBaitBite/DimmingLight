#include "MainMenuHUD.h"

#include "MainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Logging/StructuredLog.h"


void AMainMenuHUD::BeginPlay() {
	Super::BeginPlay();

	// Main Menu Widget
	if (!MainMenuTemplate) {
		UE_LOGFMT(LogTemp, Error, "AMainMenuHUD Error | BeginPlay: Main Menu class is null");
		return;
	}
	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuTemplate);
	if (!MainMenuWidget) {
		UE_LOGFMT(LogTemp, Error, "AMainMenuHUD Error | BeginPlay: Main Menu Widget could not be created");
		return;
	}
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}
