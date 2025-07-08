#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecialAttackArea.generated.h"


class AEnemyCharacterBase;
class IDamageable;
class USphereComponent;

UCLASS()
class DIMMINGLIGHT_API ASpecialAttackArea : public AActor {
	GENERATED_BODY()

	public:
		ASpecialAttackArea();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		// SPECIAL ATTACK AREA
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Special Attack Area")
		float AreaDamage = 5.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Special Attack Area")
		float AreaLifetime = 5.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Special Attack Area")
		float AreaTickDelay = 1.f;

		// Time variables
		float AreaCurrentTime = 0.f;
		float AreaTickCurrentTime = 0.f;

		// COMPONENTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
		USphereComponent* CollisionComponent;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComponent;

		// EFFECTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* AreaLoopFX;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* AreaFlashFX;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* AreaParticleComponent;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* FlashParticleComponent;

		// SOUND
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* AreaLoopSound;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* AreaFlashSound;

	private:
		// OVERLAP ACTORS
		UPROPERTY(VisibleAnywhere, Category = "Overlap")
		TArray<AEnemyCharacterBase*> OverlappingActors;

		// EVENTS
		UFUNCTION()
		void OnAreaOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

		UFUNCTION()
		void OnEndAreaOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);
};
