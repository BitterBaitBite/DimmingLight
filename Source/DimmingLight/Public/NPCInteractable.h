#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "NPCBase.h"
#include "NPCInteractable.generated.h"


UCLASS()
class DIMMINGLIGHT_API ANPCInteractable : public ANPCBase, public IInteractable {
	GENERATED_BODY()

	public:
		ANPCInteractable();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		// INTERACTABLE
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
		FString InteractionText = TEXT("Interaction text");

		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual FText GetInteraction_Implementation() override;
		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual void Interact_Implementation(AActor* InteractionActor) override;
		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual void OnFocused_Implementation() override;
		UFUNCTION(BlueprintCallable, Category="Interaction")
		virtual void OnUnfocused_Implementation() override;
};
