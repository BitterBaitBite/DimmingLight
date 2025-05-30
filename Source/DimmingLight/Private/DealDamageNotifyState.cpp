#include "DealDamageNotifyState.h"

#include "EnemyCharacterBase.h"
#include "MainCharacter.h"
#include "Logging/StructuredLog.h"

void UDealDamageNotifyState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
) {
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) {
		UE_LOGFMT(LogTemp, Error, "UDealDamageNotifyState Error | NotifyTick: Owner is null");
		return;
	}

	EnemyCharacter = Cast<AEnemyCharacterBase>(Owner);
	if (!EnemyCharacter) {
		UE_LOGFMT(LogTemp, Error, "UDealDamageNotifyState Error | NotifyTick: Owner is not an enemy character");
	}
}

void UDealDamageNotifyState::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference
) {
	if (!EnemyCharacter) return;

	FVector Start = MeshComp->GetSocketLocation(EnemyCharacter->DamageOrigin_Socket);
	FVector End = MeshComp->GetSocketLocation(EnemyCharacter->DamageEnd_Socket);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(EnemyCharacter);

	if (EnemyCharacter->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, QueryParams)) {
		AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(Hit.GetActor());
		if (!PlayerCharacter) return;

		float Damage = EnemyCharacter->GetDamage();
		PlayerCharacter->ReceiveDamage(Damage, EnemyCharacter->GetDamageType());
	}
}

void UDealDamageNotifyState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
