#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DealDamageNotifyState.generated.h"


class AEnemyCharacterBase;

UCLASS()
class DIMMINGLIGHT_API UDealDamageNotifyState : public UAnimNotifyState {
	GENERATED_BODY()

	public:
		virtual void NotifyBegin(
			USkeletalMeshComponent* MeshComp,
			UAnimSequenceBase* Animation,
			float TotalDuration,
			const FAnimNotifyEventReference& EventReference
		) override;

		virtual void NotifyTick(
			USkeletalMeshComponent* MeshComp,
			UAnimSequenceBase* Animation,
			float FrameDeltaTime,
			const FAnimNotifyEventReference& EventReference
		) override;

		virtual void NotifyEnd(
			USkeletalMeshComponent* MeshComp,
			UAnimSequenceBase* Animation,
			const FAnimNotifyEventReference& EventReference
		) override;

	private:
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AEnemyCharacterBase> EnemyCharacter;
};
