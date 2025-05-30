#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"


class AMainCharacter;
class UImage;
class UTextBlock;

UCLASS()
class DIMMINGLIGHT_API UInteractWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Glass"))
		TObjectPtr<UTextBlock> InteractionText;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AMainCharacter> OwnerCharacter = nullptr;

		UFUNCTION()
		void OnInteractFocusChange(AActor* NewInteractable);
};
