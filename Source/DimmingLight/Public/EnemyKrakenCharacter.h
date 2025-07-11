#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyKrakenCharacter.generated.h"

UCLASS()
class DIMMINGLIGHT_API AEnemyKrakenCharacter : public ACharacter {
	GENERATED_BODY()

	public:
		AEnemyKrakenCharacter();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<USceneComponent> Root;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

		// Colliders
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> FrontLeftLeg;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> FrontRightLeg;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> MiddleLeftLeg;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> MiddleRightLeg;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> BackLeftLeg;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> BackRightLeg;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> FrontBody;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> BackBody;

		// Audio & Anim components
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UAudioComponent> AudioComponent;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TObjectPtr<UAnimInstance> AnimInstance;
};
