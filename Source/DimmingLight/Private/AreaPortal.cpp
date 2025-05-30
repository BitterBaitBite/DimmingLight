#include "AreaPortal.h"

#include "ELevelType.h"
#include "MainGameInstance.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


AAreaPortal::AAreaPortal() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(RootComponent);
	PortalMesh->SetHiddenInGame(true);
}

void AAreaPortal::BeginPlay() {
	Super::BeginPlay();

	GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "AAreaPortal Error | BeginPlay: Could not find Game Instance");
	}
}

FText AAreaPortal::GetInteraction_Implementation() {
	return FText::FromString(TEXT("Travel to Combat Area"));
}

void AAreaPortal::Interact_Implementation(AActor* InteractionActor) {
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalInteractionFX, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PortalInteractionSound, GetActorLocation());

	FTimerHandle PortalTimer;
	GetWorld()->GetTimerManager().SetTimer(
		PortalTimer,
		[this] {
			if (LevelName != NAME_None) GameInstance->LoadLevelByName(LevelName);
			else GameInstance->LoadLevelByType(ELevelType::CombatArea);
		},
		1.5f,
		false
	);
}

void AAreaPortal::OnFocused_Implementation() {
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalFocusFX, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PortalFocusSound, GetActorLocation());
}
