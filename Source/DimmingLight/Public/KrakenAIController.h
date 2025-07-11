#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KrakenAIController.generated.h"

UCLASS()
class DIMMINGLIGHT_API AKrakenAIController : public AAIController {
	GENERATED_BODY()

	public:
		AKrakenAIController();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
};
