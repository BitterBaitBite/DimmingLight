#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"


UINTERFACE(Blueprintable)
class UInteractable : public UInterface {
	GENERATED_BODY()
};

class DIMMINGLIGHT_API IInteractable {
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		FText GetInteraction();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void Interact(AActor* InteractionActor);

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnFocused();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnUnfocused();
};
