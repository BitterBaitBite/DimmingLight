#include "SpecialAttackArea.h"

#include "Damageable.h"
#include "EnemyCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Particles/ParticleSystemComponent.h"


ASpecialAttackArea::ASpecialAttackArea() {
	PrimaryActorTick.bCanEverTick = true;

	// Area Collision and Set Root
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName("OverlapAllDynamic");
	SetRootComponent(CollisionComponent);

	// Mesh (for testing purposes)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName("NoCollision");
	MeshComponent->SetHiddenInGame(true);
}

void ASpecialAttackArea::BeginPlay() {
	Super::BeginPlay();

	FlashParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
		this,
		AreaFlashFX,
		GetActorLocation(),
		GetActorRotation()
	);
	AreaParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
		this,
		AreaLoopFX,
		GetActorLocation(),
		GetActorRotation()
	);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpecialAttackArea::OnAreaOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASpecialAttackArea::OnEndAreaOverlap);
}

void ASpecialAttackArea::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AreaCurrentTime += DeltaTime;
	if (AreaCurrentTime >= AreaLifetime) {
		if (FlashParticleComponent) FlashParticleComponent->Deactivate();
		if (AreaParticleComponent) AreaParticleComponent->Deactivate();
		Destroy();
		return;
	}

	AreaTickCurrentTime += DeltaTime;
	if (AreaTickCurrentTime >= AreaTickDelay) {
		AreaTickCurrentTime = 0;

		if (OverlappingActors.IsEmpty()) return;
		for (AEnemyCharacterBase* OverlappingEnemy : OverlappingActors) {
			UGameplayStatics::ApplyDamage(
				OverlappingEnemy,
				AreaDamage,
				GetInstigatorController(),
				this,
				nullptr
			);
		}
	}
}

void ASpecialAttackArea::OnAreaOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.f,
		FColor::Orange,
		FString::Printf(TEXT("Overlap actor: %s"), *OtherActor->GetName())
	);

	if (AEnemyCharacterBase* OverlappingEnemy = Cast<AEnemyCharacterBase>(OtherActor)) {
		OverlappingActors.AddUnique(OverlappingEnemy);

		UGameplayStatics::ApplyDamage(
			OverlappingEnemy,
			AreaDamage,
			GetInstigatorController(),
			this,
			nullptr
		);

		GEngine->AddOnScreenDebugMessage(
			-1,
			10.f,
			FColor::Orange,
			FString::Printf(TEXT("Overlap enemy: %s"), *OverlappingEnemy->GetName())
		);
	}
}

void ASpecialAttackArea::OnEndAreaOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
	AEnemyCharacterBase* OverlappingEnemy = Cast<AEnemyCharacterBase>(OtherActor);
	if (OverlappingActors.Contains(OverlappingEnemy)) {
		OverlappingActors.Remove(OverlappingEnemy);
	}
}
