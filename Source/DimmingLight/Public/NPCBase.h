#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"


UCLASS()
class DIMMINGLIGHT_API ANPCBase : public ACharacter {
	GENERATED_BODY()

	public:
		ANPCBase();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
		FString NPCName = TEXT("NPC Base");
};
