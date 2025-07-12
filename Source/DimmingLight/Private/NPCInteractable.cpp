#include "NPCInteractable.h"


ANPCInteractable::ANPCInteractable() {
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCInteractable::BeginPlay() {
	Super::BeginPlay();
}

void ANPCInteractable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

FText ANPCInteractable::GetInteraction_Implementation() {
	return FText::FromString(FString::Printf(TEXT("%s %s"), *InteractionText, *NPCName));
}

void ANPCInteractable::Interact_Implementation(AActor* InteractionActor) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Interactuando con %s"), *NPCName));
}

void ANPCInteractable::OnFocused_Implementation() {
	GetMesh()->SetRenderCustomDepth(true);
}

void ANPCInteractable::OnUnfocused_Implementation() {
	GetMesh()->SetRenderCustomDepth(false);
}
