#include "InteractableChest.h"

#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "Components/RectLightComponent.h"


AInteractableChest::AInteractableChest() {
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	LidPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Lid Pivot"));
	LidPivot->SetupAttachment(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lid Mesh"));
	LidMesh->SetupAttachment(LidPivot);

	ChestLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Chest Light"));
	ChestLight->SetupAttachment(RootComponent);
	ChestLight->Intensity = 0.f;
}

void AInteractableChest::BeginPlay() {
	Super::BeginPlay();

	ChestLight->Intensity = 0.f;
}

void AInteractableChest::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!bIsOpen) return;

	if (ChestLight->Intensity < OpenLightIntensity) {
		ChestLight->Intensity += IntensityChangeSpeed * DeltaTime;
	}
	else {
		ChestLight->Intensity = OpenLightIntensity;
	}

	FRotator CurrentLidRotation = LidPivot->GetRelativeRotation();
	if (CurrentLidRotation.Pitch < OpenLidRotation.Pitch) {
		float PitchDelta = OpenLidSpeed * DeltaTime;
		FRotator NewLidRotation = CurrentLidRotation + FRotator(PitchDelta, 0.f, 0.f);

		LidPivot->SetRelativeRotation(NewLidRotation);
	}
	else {
		LidPivot->SetRelativeRotation(OpenLidRotation);
	}
}

void AInteractableChest::Interact_Implementation(AActor* InteractionActor) {
	IInteractable::Interact_Implementation(InteractionActor);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Interacting with chest");
	if (bIsOpen) {
		return;
	}

	bIsOpen = true;
}

FText AInteractableChest::GetInteraction_Implementation() {
	if (bIsOpen) {
		return FText::FromString(TEXT("Chest is open"));
	}

	return FText::FromString(TEXT("Open chest"));
}

void AInteractableChest::OnFocused_Implementation() {
	IInteractable::OnFocused_Implementation();
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Focus on chest");
}

void AInteractableChest::OnUnfocused_Implementation() {
	IInteractable::OnUnfocused_Implementation();
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Chest lost focus");
}
