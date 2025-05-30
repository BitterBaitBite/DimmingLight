#include "LightDrop.h"

#include "MainCharacter.h"
#include "MainGameInstance.h"
#include "MainGameState.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "Logging/StructuredLog.h"
#include "Particles/ParticleSystemComponent.h"


ALightDrop::ALightDrop() {
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SphereCollisionComponent->SetCollisionProfileName("OverlapAllDynamic");
	SetRootComponent(SphereCollisionComponent);

	LightDropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightDropMesh"));
	LightDropMesh->SetupAttachment(RootComponent);
	LightDropMesh->SetHiddenInGame(true);
	LightDropMesh->SetCollisionProfileName("NoCollision");

	LightDropParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LightDropParticleSystem"));
	LightDropParticleSystem->SetupAttachment(RootComponent);
	LightDropParticleSystem->SetCollisionProfileName("NoCollision");
}

void ALightDrop::BeginPlay() {
	Super::BeginPlay();

	AddedLightInSeconds += FMath::RandRange(-5.0f, +5.0f);

	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ALightDrop::OnOverlap);

	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimer,
		[this] { Destroy(); },
		10.f,
		false
	);
}

void ALightDrop::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(OtherActor)) {
		AMainGameState* GameState = Cast<AMainGameState>(GetWorld()->GetGameState());
		if (!GameState) {
			UE_LOGFMT(LogTemp, Error, "ALightDrop Error | OnOverlap: Could not find Game State");
		}
		else {
			GameState->AddLightInSeconds(AddedLightInSeconds);
			Destroy();
		}
	}
}
