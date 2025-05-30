#include "EnemyCharacterBase.h"

#include "LightDrop.h"
#include "MainGameInstance.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


AEnemyCharacterBase::AEnemyCharacterBase() {
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyCharacterBase::BeginPlay() {
	Super::BeginPlay();

	Health = MaxHealth;
}

int32 AEnemyCharacterBase::GetMaxHealth() {
	return MaxHealth;
}

int32 AEnemyCharacterBase::GetHealth() {
	return Health;
}

float AEnemyCharacterBase::TakeDamage(
	float DamageAmount,
	const FDamageEvent& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
) {
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("Health: %i - %f = %f"), Health, DamageAmount, Health - DamageAmount)
	);
	Health -= DamageAmount;
	if (Health <= 0) {
		Death();
	}

	return DamageAmount;
}

void AEnemyCharacterBase::Death() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	FOnMontageEnded OnMontageEndedDelegate;
	OnMontageEndedDelegate.BindUFunction(this, FName("OnDeathMontageEnd"));
	AnimInstance->Montage_Play(DeathAnimation);
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, DeathAnimation);
}

void AEnemyCharacterBase::OnDeathMontageEnd() {
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "AEnemyCharacterBase Error | OnDeathMontageEnd: Could not find Game Instance");
	}
	else {
		GameInstance->AddCurrency(CurrencyReward);
	}

	FVector ParticlesOrigin = GetMesh()->GetSocketLocation(FXRoot_Socket);
	FRotator ParticlesRotation = GetMesh()->GetSocketRotation(FXRoot_Socket);

	UGameplayStatics::SpawnEmitterAtLocation(this, DeathFX, ParticlesOrigin, ParticlesRotation);

	// Random probability to spawn a drop on death
	if (FMath::RandRange(0.0f, 1.0f) > LightDropChance) {
		Destroy();
		return;
	}

	// Origin Socket
	FVector DropSpawnLocation = GetMesh()->GetSocketLocation(FXRoot_Socket);
	FRotator DropSpawnRotation = GetMesh()->GetSocketRotation(FXRoot_Socket);

	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ALightDrop* LightDrop = GetWorld()->SpawnActor<ALightDrop>(
		LightDropClass,
		DropSpawnLocation,
		DropSpawnRotation,
		SpawnParams
	);

	Destroy();
}

float AEnemyCharacterBase::GetDamage() {
	return Damage;
}

EDamageType AEnemyCharacterBase::GetDamageType() {
	return DamageType;
}
