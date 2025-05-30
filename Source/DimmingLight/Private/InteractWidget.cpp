#include "InteractWidget.h"

#include "Interactable.h"
#include "MainCharacter.h"
#include "Components/TextBlock.h"
#include "Logging/StructuredLog.h"


void UInteractWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetIsFocusable(false);

	APawn* OwnerPawn = GetOwningPlayerPawn();

	if (!OwnerPawn) {
		UE_LOGFMT(LogTemp, Error, "UInteractWidget Error | NativeConstruct: Failed to get owner pawn");
		return;
	}

	OwnerCharacter = Cast<AMainCharacter>(OwnerPawn);

	if (!OwnerCharacter) {
		UE_LOGFMT(LogTemp, Error, "UInteractWidget Error | NativeConstruct: Failed to get the owner character");
		return;
	}

	OwnerCharacter->InteractFocusChangeDelegate.AddDynamic(this, &UInteractWidget::OnInteractFocusChange);
}

void UInteractWidget::OnInteractFocusChange(AActor* NewInteractable) {
	if (!NewInteractable) {
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (InteractionText && NewInteractable->Implements<UInteractable>()) {
		InteractionText->SetText(IInteractable::Execute_GetInteraction(NewInteractable));

		SetVisibility(ESlateVisibility::Visible);
	}
}
