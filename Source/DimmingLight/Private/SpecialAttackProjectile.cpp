#include "SpecialAttackProjectile.h"

#include "EnemyCharacterBase.h"
#include "NPCBase.h"
#include "SpecialAttackArea.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


ASpecialAttackProjectile::ASpecialAttackProjectile() {
	PrimaryActorTick.bCanEverTick = false;

	// Projectile Collision and Set Root
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(ProjectileRadius);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

	// Light component
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	LightComponent->SetupAttachment(RootComponent);

	InitialLifeSpan = ProjectileLifetime;
}

void ASpecialAttackProjectile::BeginPlay() {
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
	if (ProjectileFX) {
		ProjectileParticleComponent = UGameplayStatics::SpawnEmitterAttached(ProjectileFX, RootComponent);
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &ASpecialAttackProjectile::OnProjectileHit);
	SetLifeSpan(InitialLifeSpan);
}

void ASpecialAttackProjectile::FireProjectile(const FVector& Direction) {
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileInitSpeed;
	ProjectileMovement->UpdateComponentVelocity();
	ProjectileMovement->Activate();

	// Sound
	if (MuzzleFlashSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MuzzleFlashSound, GetActorLocation());
	}
}

void ASpecialAttackProjectile::FireHomingProjectile(const AActor* TargetActor) {
	ProjectileMovement->ProjectileGravityScale = 0.2f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	ProjectileMovement->HomingAccelerationMagnitude = ProjectileHomingAcceleration;
	ProjectileMovement->Activate();

	// Sound
	if (MuzzleFlashSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MuzzleFlashSound, GetActorLocation());
	}
}

void ASpecialAttackProjectile::OnProjectileHit(
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

		// Spawn Damage Area
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		if (SpecialAttackArea) {
			ASpecialAttackArea* DamageArea = GetWorld()->SpawnActor<ASpecialAttackArea>(
				SpecialAttackArea,
				Hit.ImpactPoint,
				FRotator(0.f, GetActorRotation().Yaw, 0.f),
				SpawnParams
			);
		}

		Destroy();
		return;
	}

	UE_LOGFMT(
		LogTemp,
		Error,
		"ASpecialAttackProjectile Warning | Notify Hit: No particle systems available"
	);

	Destroy();
}
