#include "EnemyKrakenCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


AEnemyKrakenCharacter::AEnemyKrakenCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = GetMesh();
	// SkeletalMesh->SetupAttachment(Root);

	// Colliders
	FrontLeftLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrontLeftLeg"));
	FrontLeftLeg->SetupAttachment(SkeletalMesh, TEXT("F_L_Leg"));
	FrontRightLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrontRightLeg"));
	FrontRightLeg->SetupAttachment(SkeletalMesh, TEXT("F_R_Leg"));
	MiddleLeftLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MiddleLeftLeg"));
	MiddleLeftLeg->SetupAttachment(SkeletalMesh, TEXT("M_L_Leg"));
	MiddleRightLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MiddleRightLeg"));
	MiddleRightLeg->SetupAttachment(SkeletalMesh, TEXT("M_R_Leg"));
	BackLeftLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BackLeftLeg"));
	BackLeftLeg->SetupAttachment(SkeletalMesh, TEXT("B_L_Leg"));
	BackRightLeg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BackRightLeg"));
	BackRightLeg->SetupAttachment(SkeletalMesh, TEXT("B_R_Leg"));
	FrontBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FrontBody"));
	FrontBody->SetupAttachment(SkeletalMesh, TEXT("F_Body"));
	BackBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BackBody"));
	BackBody->SetupAttachment(SkeletalMesh, TEXT("B_Body"));
}

void AEnemyKrakenCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AEnemyKrakenCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
