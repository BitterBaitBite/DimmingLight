#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "AreaPortal.generated.h"


class USphereComponent;
class UMainGameInstance;

UCLASS()
class DIMMINGLIGHT_API AAreaPortal : public AActor, public IInteractable {
	GENERATED_BODY()

	public:
		AAreaPortal();
		virtual void BeginPlay() override;

		// COMPONENTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<USphereComponent> CollisionComponent = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<UStaticMeshComponent> PortalMesh = nullptr;

		// LEVEL PORTAL
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level Portal")
		FName LevelName = NAME_None;

		// EFFECTS
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
		TObjectPtr<UParticleSystem> PortalFocusFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
		TObjectPtr<UParticleSystem> PortalInteractionFX = nullptr;

		// SOUNDS
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
		USoundBase* PortalFocusSound = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
		USoundBase* PortalInteractionSound = nullptr;

		// INTERACTABLE
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
		FString InteractionText = TEXT("Interaction text");

		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual FText GetInteraction_Implementation() override;
		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual void Interact_Implementation(AActor* InteractionActor) override;
		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual void OnFocused_Implementation() override;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UMainGameInstance> GameInstance = nullptr;
};
