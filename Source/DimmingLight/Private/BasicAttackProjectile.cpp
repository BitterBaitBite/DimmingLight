#include "BasicAttackProjectile.h"

#include "EnemyCharacterBase.h"
#include "NPCBase.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


ABasicAttackProjectile::ABasicAttackProjectile() {
	PrimaryActorTick.bCanEverTick = false;

	// Projectile Collision and Set Root
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(ProjectileRadius);
	CollisionComponent->SetCollisionProfileName("BlockAll");
	SetRootComponent(CollisionComponent);

	// Mesh (for testing purposes)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetHiddenInGame(true);

	// Projectile movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = ProjectileInitSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = ProjectileLifetime;
}

void ABasicAttackProjectile::BeginPlay() {
	Super::BeginPlay();

	if (GetOwner()) {
		CollisionComponent->IgnoreActorWhenMoving(Owner, true);
	}
	if (MuzzleFlashFX) {
		MuzzleFlashParticleComponent = UGameplayStatics::SpawnEmitterAttached(MuzzleFlashFX, RootComponent);
	}
	if (TrailFX) {
		TrailParticleComponent = UGameplayStatics::SpawnEmitterAttached(TrailFX, RootComponent);
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &ABasicAttackProjectile::OnProjectileHit);
	SetLifeSpan(InitialLifeSpan);
}

void ABasicAttackProjectile::FireProjectile(const FVector& Direction) {
	ProjectileMovement->Velocity = Direction * ProjectileInitSpeed;
	ProjectileMovement->UpdateComponentVelocity();
	ProjectileMovement->Activate();

	// Sound
	if (MuzzleFlashSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MuzzleFlashSound, GetActorLocation());
	}
}

void ABasicAttackProjectile::OnProjectileHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	if (!OtherActor || OtherActor == GetOwner()) return;

	AEnemyCharacterBase* EnemyCharacter = Cast<AEnemyCharacterBase>(OtherActor);
	if (EnemyCharacter && HitEnemyFX) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitEnemyFX,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);

		UGameplayStatics::ApplyPointDamage(
			EnemyCharacter,
			ProjectileDamage,
			GetVelocity().GetSafeNormal(),
			Hit,
			GetInstigatorController(),
			this,
			nullptr
		);

		// Sound
		if (EntityHitSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EntityHitSound, Hit.ImpactPoint);
		}

		Destroy();
		return;
	}

	ANPCBase* NPC = Cast<ANPCBase>(OtherActor);
	if (NPC && HitNpcFX) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitNpcFX,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);

		// Sound
		if (EntityHitSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EntityHitSound, Hit.ImpactPoint);
		}

		Destroy();
		return;
	}

	if (HitDefaultFX) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitDefaultFX,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);

		// Sound
		if (WorldHitSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WorldHitSound, Hit.ImpactPoint);
		}

		Destroy();
		return;
	}

	UE_LOGFMT(
		LogTemp,
		Error,
		"ABasicAttackProjectile Warning | Notify Hit: No particle systems available"
	);

	Destroy();
}
