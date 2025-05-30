#include "AIEnemyController.h"

#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


AAIEnemyController::AAIEnemyController() {
	PrimaryActorTick.bCanEverTick = false;
}

void AAIEnemyController::BeginPlay() {
	Super::BeginPlay();

	ControlledPawn = GetPawn();
	if (!ControlledPawn) {
		UE_LOGFMT(LogTemp, Error, "AAIEnemyController Error | BeginPlay: Failed to get controlled pawn");
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) {
		UE_LOGFMT(LogTemp, Error, "AAIEnemyController Error | BeginPlay: Failed to get Player Pawn");
		return;
	}

	PlayerCharacter = Cast<AMainCharacter>(PlayerPawn);
	if (!PlayerCharacter) {
		UE_LOGFMT(LogTemp, Error, "AAIEnemyController Error | BeginPlay: Failed to get Player Character");
		return;
	}

	PlayerCharacter->PlayerDeathDelegate.AddDynamic(this, &AAIEnemyController::OnPlayerDeath);
	bCanAttack = true;

	if (!BlackboardAsset) {
		UE_LOGFMT(LogTemp, Error, "AAIEnemyController Error | BeginPlay: Failed to get the Blackboard Asset");
		return;
	}
	bool IsBlackboardUsed = UseBlackboard(BlackboardAsset, BlackboardComponent);
	if (!IsBlackboardUsed) {
		UE_LOGFMT(LogTemp, Error, "AAIEnemyController Error | BeginPlay: Failed to use the specified Blackboard");
	}
	BlackboardComponent->SetValueAsBool(BB_CanAttack, bCanAttack);

	if (!BehaviorTree) {
		UE_LOGFMT(LogTemp, Error, "AAIEnemyController Error | BeginPlay: Failed to get the Behavior Tree");
		return;
	}
	RunBehaviorTree(BehaviorTree);
}

bool AAIEnemyController::GetCanAttack() {
	return bCanAttack;
}

void AAIEnemyController::OnPlayerDeath() {
	bCanAttack = false;
}
