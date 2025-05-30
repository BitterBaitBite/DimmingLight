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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hablar con %s"), *NPCName));
	// TODO render outline?
}

void ANPCInteractable::OnUnfocused_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("(%s fuera de foco)"), *NPCName));
	// TODO shut off outline?
}
