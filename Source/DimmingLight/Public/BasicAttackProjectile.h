#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicAttackProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DIMMINGLIGHT_API ABasicAttackProjectile : public AActor {
	GENERATED_BODY()

	public:
		ABasicAttackProjectile();
		virtual void BeginPlay() override;

		// PROJECTILE PROPERTIES
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileInitSpeed = 1000.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileMaxSpeed = 1000.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileRadius = 80.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileDamage = 10.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileLifetime = 10.f;

		// COMPONENTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovement;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
		USphereComponent* CollisionComponent;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComponent;

		// VFX
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* MuzzleFlashFX;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* TrailFX;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* HitEnemyFX;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* HitNpcFX;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* HitTerrainFX;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* HitDefaultFX;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* TrailParticleComponent;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* MuzzleFlashParticleComponent;

		// SOUND
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* MuzzleFlashSound;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* WorldHitSound;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* EntityHitSound;

		// ACTIVATE FUNCTION
		UFUNCTION(BlueprintCallable, Category = "ProjectileMovement")
		void FireProjectile(const FVector& Direction);

		// HIT EVENT
		UFUNCTION()
		void OnProjectileHit(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit
		);
};
