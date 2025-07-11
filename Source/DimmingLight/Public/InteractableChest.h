#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableChest.generated.h"


class URectLightComponent;
class UBoxComponent;

UCLASS()
class DIMMINGLIGHT_API AInteractableChest : public AActor, public IInteractable {
	GENERATED_BODY()

	public:
		AInteractableChest();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		// COMPONENTS
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<UBoxComponent> CollisionComponent = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<UStaticMeshComponent> BaseMesh = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<UStaticMeshComponent> LidMesh = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<USceneComponent> LidPivot = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
		TObjectPtr<URectLightComponent> ChestLight = nullptr;

		// INTERACTION PROPERTIES
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
		FRotator OpenLidRotation = FRotator(-110.f, 0.f, 0.f);
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
		float OpenLidSpeed = 10.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
		float OpenLightIntensity = 2000.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
		float IntensityChangeSpeed = 200.f;

		// INTERACTABLE INTERFACE
		UFUNCTION()
		virtual void Interact_Implementation(AActor* InteractionActor) override;
		UFUNCTION()
		virtual FText GetInteraction_Implementation() override;
		UFUNCTION()
		virtual void OnFocused_Implementation() override;
		UFUNCTION()
		virtual void OnUnfocused_Implementation() override;

	protected:
		bool bIsOpen = false;
};
