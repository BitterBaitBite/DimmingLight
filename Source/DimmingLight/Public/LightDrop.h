#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightDrop.generated.h"


class USphereComponent;

UCLASS()
class DIMMINGLIGHT_API ALightDrop : public AActor {
	GENERATED_BODY()

	public:
		ALightDrop();
		UFUNCTION()
		virtual void BeginPlay() override;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Light")
		float AddedLightInSeconds = 20.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
		TObjectPtr<USphereComponent> SphereCollisionComponent = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
		TObjectPtr<UStaticMeshComponent> LightDropMesh = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
		TObjectPtr<UParticleSystemComponent> LightDropParticleSystem = nullptr;

		// EVENTS
		UFUNCTION(BlueprintCallable)
		void OnOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);
};
