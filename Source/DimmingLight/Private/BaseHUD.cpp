#include "DimmingLight/Public/BaseHUD.h"

#include "CurrencyWidget.h"
#include "HealthWidget.h"
#include "InteractWidget.h"
#include "LuminosityWidget.h"
#include "MainCharacter.h"
#include "ShopWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

class ULuminosityWidget;
class UHealthWidget;

void ABaseHUD::BeginPlay() {
	Super::BeginPlay();

	// Health Widget
	if (!HealthWidgetTemplate) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Health widget template is null");
		return;
	}
	HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetTemplate);
	if (!HealthWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Health widget could not be created");
		return;
	}
	HealthWidget->AddToViewport();
	HealthWidget->SetVisibility(ESlateVisibility::Visible);

	// Luminosity Widget
	if (!LuminosityWidgetTemplate) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Luminosity widget template is null");
		return;
	}
	LuminosityWidget = CreateWidget<ULuminosityWidget>(GetWorld(), LuminosityWidgetTemplate);
	if (!LuminosityWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Luminosity widget could not be created");
		return;
	}
	LuminosityWidget->AddToViewport();
	LuminosityWidget->SetVisibility(ESlateVisibility::Visible);

	// Currency Widget
	if (!CurrencyWidgetTemplate) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Currency widget template is null");
		return;
	}
	CurrencyWidget = CreateWidget<UCurrencyWidget>(GetWorld(), CurrencyWidgetTemplate);
	if (!CurrencyWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Currency widget could not be created");
		return;
	}
	CurrencyWidget->AddToViewport();
	CurrencyWidget->SetVisibility(ESlateVisibility::Visible);

	// Interaction Widget
	if (!InteractWidgetTemplate) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Interaction widget template is null");
		return;
	}
	InteractWidget = CreateWidget<UInteractWidget>(GetWorld(), InteractWidgetTemplate);
	if (!InteractWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Interaction widget could not be created");
		return;
	}
	InteractWidget->AddToViewport();
	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);

	// Shop Widget
	if (!ShopWidgetTemplate) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Shop widget template is null");
		return;
	}
	ShopWidget = CreateWidget<UShopWidget>(GetWorld(), ShopWidgetTemplate);
	if (!ShopWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Shop widget could not be created");
		return;
	}
	ShopWidget->AddToViewport();
	ShopWidget->SetVisibility(ESlateVisibility::Collapsed);

	// Get player
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Failed to get player pawn");
		return;
	}

	AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(PlayerPawn);
	if (!PlayerCharacter) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | BeginPlay: Failed to get player character");
		return;
	}

	// Player Death event bind
	PlayerCharacter->PlayerDeathDelegate.AddDynamic(this, &ABaseHUD::OnPlayerDeath);
}

void ABaseHUD::OpenShop() {
	if (!ShopWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | OpenShop: Shop widget is null");
		return;
	}

	FInputModeGameAndUI GameAndUIMode;
	PlayerOwner->SetInputMode(GameAndUIMode);
	PlayerOwner->bShowMouseCursor = true;
	ShopWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABaseHUD::CloseShop() {
	if (!ShopWidget) {
		UE_LOGFMT(LogTemp, Error, "BaseHUD Error | CloseShop: Shop widget is null");
		return;
	}

	FInputModeGameOnly GameOnlyMode;
	PlayerOwner->bShowMouseCursor = false;
	PlayerOwner->SetInputMode(GameOnlyMode);
	ShopWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ABaseHUD::OnPlayerDeath() {
	HealthWidget->SetVisibility(ESlateVisibility::Collapsed);
	LuminosityWidget->SetVisibility(ESlateVisibility::Collapsed);
	CurrencyWidget->SetVisibility(ESlateVisibility::Collapsed);
	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
	ShopWidget->SetVisibility(ESlateVisibility::Collapsed);
}
