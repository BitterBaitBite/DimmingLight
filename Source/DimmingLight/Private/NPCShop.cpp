#include "NPCShop.h"

#include "BaseHUD.h"
#include "Logging/StructuredLog.h"


ANPCShop::ANPCShop() {
	PrimaryActorTick.bCanEverTick = false;

	NPCName = TEXT("NPC Shop");
	InteractionText = TEXT("Comprar a");
}

void ANPCShop::BeginPlay() {
	Super::BeginPlay();

	AHUD* CurrentHUD = GetWorld()->GetFirstPlayerController()->GetHUD();
	if (!CurrentHUD) {
		UE_LOGFMT(LogTemp, Error, "ANPCShop Error | OnUnfocused: could not find any HUD");
		return;
	}
	ABaseHUD* BaseHUD = Cast<ABaseHUD>(CurrentHUD);
	if (!BaseHUD) {
		UE_LOGFMT(LogTemp, Error, "ANPCShop Error | OnUnfocused: could not find Base HUD");
		return;
	}
	HUD = BaseHUD;
}

FUpgradeData ANPCShop::GetAvailableUpgrades() {
	return AvailableUpgrades;
}

// INTERACTABLE
void ANPCShop::Interact_Implementation(AActor* InteractionActor) {
	UpgradeAvailabilityChangeDelegate.Broadcast(GetAvailableUpgrades());
	HUD->OpenShop();
}

void ANPCShop::OnUnfocused_Implementation() {
	HUD->CloseShop();
}
