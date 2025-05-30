#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UImage;
class AMainCharacter;

UCLASS()
class DIMMINGLIGHT_API UHealthWidget : public UUserWidget {
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Frame"))
		TObjectPtr<UImage> Frame;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Glass"))
		TObjectPtr<UImage> Glass;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="Orb"))
		TObjectPtr<UImage> Orb;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AMainCharacter> OwnerCharacter = nullptr;

		UFUNCTION(BlueprintCallable)
		void OnHealthChange(int32 NewHealth);
};
