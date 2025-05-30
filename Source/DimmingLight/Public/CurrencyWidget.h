#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrencyWidget.generated.h"


class UTextBlock;

UCLASS()
class DIMMINGLIGHT_API UCurrencyWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		UFUNCTION()
		virtual void NativeConstruct() override;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets", meta=(BindWidget="CurrencyText"))
		TObjectPtr<UTextBlock> CurrencyText = nullptr;

		UFUNCTION()
		void OnCurrencyChange(int32 NewCurrencyValue);
};
