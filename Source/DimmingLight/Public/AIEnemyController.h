#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AIEnemyController.generated.h"


class AMainCharacter;

UCLASS()
class DIMMINGLIGHT_API AAIEnemyController : public AAIController {
	GENERATED_BODY()

	public:
		AAIEnemyController();
		virtual void BeginPlay() override;

		// BLACKBOARD
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Behavior")
		TObjectPtr<UBehaviorTree> BehaviorTree;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Behavior")
		UBlackboardComponent* BlackboardComponent;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Behavior")
		TObjectPtr<UBlackboardData> BlackboardAsset;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Behavior")
		FName BB_CanAttack = TEXT("bCanAttack");

		UFUNCTION(BlueprintCallable)
		bool GetCanAttack();

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Enemy properties", meta=(AllowPrivateAccess=true))
		bool bCanAttack = false;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Enemy properties", meta=(AllowPrivateAccess=true))
		TObjectPtr<APawn> ControlledPawn = nullptr;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player", meta=(AllowPrivateAccess=true))
		TObjectPtr<AMainCharacter> PlayerCharacter = nullptr;

		UFUNCTION(BlueprintCallable, Category="Events")
		void OnPlayerDeath();
};
