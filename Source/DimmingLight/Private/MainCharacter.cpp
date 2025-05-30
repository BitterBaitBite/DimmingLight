#include "MainCharacter.h"

#include "BasicAttackProjectile.h"
#include "EDamageType.h"
#include "ELevelType.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EStatUpgradeType.h"
#include "Interactable.h"
#include "MainGameInstance.h"
#include "MainGameState.h"
#include "MainPlayerController.h"
#include "SpecialAttackProjectile.h"
#include "Camera/CameraActor.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"


AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	// Audio Component for Voice Lines
	VoiceAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceAudioComponent"));
	VoiceAudioComponent->SetupAttachment(RootComponent);
	VoiceAudioComponent->SetAutoActivate(false);
	VoiceAudioComponent->SetUISound(false);

	// Light Source Mesh Component
	LightSourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSourceMesh"));
	LightSourceMesh->SetupAttachment(RootComponent);
	LightSourceMesh->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

	// Light Component for Luminosity Mechanic
	CharacterSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("CharacterSpotlight"));
	CharacterSpotlight->SetupAttachment(LightSourceMesh ? LightSourceMesh : RootComponent);
	CharacterSpotlight->SetIntensity(LightIntensity);
	CharacterSpotlight->SetLightFColor(LightColor);
	CharacterSpotlight->SetTemperature(LightTemperature);
	CharacterSpotlight->SetInnerConeAngle(LightInnerAngleMaximum);
	CharacterSpotlight->SetOuterConeAngle(LightInnerAngleMaximum + LightOuterAngleOffset);
	CharacterSpotlight->SetAttenuationRadius(LightAttenuationRadius);
	CharacterSpotlight->SetUseTemperature(true);
	CharacterSpotlight->SetVisibility(true);

	// Light Particles Effect Component
	LightEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LightEffectComponent"));
	LightEffectComponent->SetupAttachment(LightSourceMesh ? LightSourceMesh : RootComponent);

	// Interact Box Component
	InteractBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);
	InteractBoxComponent->SetBoxExtent(FVector(60.f, 60.f, 100.f));
	InteractBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractBoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractBoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	InteractBoxComponent->SetGenerateOverlapEvents(true);

	// Initialize Stats
	BasicAttackUpgradedDamage = BasicAttackInitialDamage;
	BasicAttackCurrentDamage = BasicAttackInitialDamage;
	SpecialAttackUpgradedDamage = SpecialAttackInitialDamage;
	SpecialAttackCurrentDamage = SpecialAttackInitialDamage;
	MaxHealthUpgraded = MaxHealthInitial;
	PhysicalResistanceUpgraded = PhysicalResistanceInitial;
	MagicResistanceUpgraded = MagicResistanceInitial;
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();

	bIsInvulnerable = false;

	// Store Game State
	GameState = Cast<AMainGameState>(GetWorld()->GetGameState());
	if (!GameState) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | BeginPlay: Could not find the correct Game State");
		return;
	}

	// Bind Luminosity Change
	GameState->LuminosityChangeDelegate.AddDynamic(this, &AMainCharacter::OnLuminosityChange);

	// Get Game Instance
	GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | BeginPlay: Could not find the game instance");
		return;
	}

	// Initialize Input
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(Controller);
	if (!PlayerController) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | BeginPlay: Could not find the player controller");
		return;
	}

	auto* LocalPlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer()
	);
	if (!LocalPlayerInputSubsystem) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | BeginPlay: Could not find the local player input subsystem");
		return;
	}

	LocalPlayerInputSubsystem->AddMappingContext(DefaultMappingContext, 0);

	// Initialize Stats
	MaxHealthUpgraded = GameInstance->GetMaxHealthUpgraded(MaxHealthInitial);
	Health = MaxHealthUpgraded;
	HealthChangeDelegate.Broadcast(Health);
	BasicAttackUpgradedDamage = GameInstance->GetBasicAttackDamageUpgraded(BasicAttackInitialDamage);
	SpecialAttackUpgradedDamage = GameInstance->GetSpecialAttackDamageUpgraded(SpecialAttackInitialDamage);
	PhysicalResistanceUpgraded = FMath::Clamp(
		GameInstance->GetPhysicalResistanceUpgraded(PhysicalResistanceInitial),
		PhysicalResistanceInitial,
		MaxPhysicResistance
	);
	MagicResistanceUpgraded = FMath::Clamp(
		GameInstance->GetMagicResistanceUpgraded(MagicResistanceInitial),
		MagicResistanceInitial,
		MaxMagicResistance
	);

	// Death Event Binding
	PlayerDeathDelegate.AddDynamic(this, &AMainCharacter::OnDeath);

	// Bind BoxComponent Overlap Events
	if (!InteractBoxComponent) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | BeginPlay: Interact box component is null");
		return;
	}
	InteractBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnInteractableEnter);
	InteractBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnInteractableExit);
}

void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateFocusInteractable();
	if (LightSourceMesh) {
		LightSourceMesh->AddWorldRotation(FRotator(0, 5.f * DeltaTime, 0));
	}
}

// SETUP INPUT
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInput) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | SetupPlayerInputComponent: Enhanced input component is null");
		return;
	}

	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainCharacter::ResetMoveDirection);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
	EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AMainCharacter::Interact);
	EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMainCharacter::Dodge);
	EnhancedInput->BindAction(ShootAction, ETriggerEvent::Started, this, &AMainCharacter::Shoot);
	EnhancedInput->BindAction(SpecialAction, ETriggerEvent::Triggered, this, &AMainCharacter::SpecialAttackStart);
	EnhancedInput->BindAction(SpecialAction, ETriggerEvent::Completed, this, &AMainCharacter::SpecialAttackRelease);
}

// AUDIO
void AMainCharacter::PlayVoiceAudio(USoundBase* VoiceAudio) {
	if (!VoiceAudio || !VoiceAudioComponent) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | PlayVoiceAudio: Voice audio or voice audio component is null");
		return;
	}

	if (VoiceAudioComponent->IsPlaying()) {
		VoiceAudioComponent->Stop();
	}

	VoiceAudioComponent->SetSound(VoiceAudio);
	VoiceAudioComponent->Play();
}

void AMainCharacter::PlaySoundEffect(USoundBase* FXAudio, FName SocketName) {
	if (!FXAudio) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | PlaySoundEffect: FX audio is null");
		return;
	}

	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FXAudio, SocketLocation);
}

void AMainCharacter::PlayParticleSystem(UParticleSystem* Effect, FName SocketName) {
	if (!Effect) {
		UE_LOGFMT(LogTemp, Error, "MainCharacter Error | PlayParticleSystem: Effect is null");
		return;
	}

	FVector SocketLocation = GetMesh()->GetSocketLocation(SocketName);
	FRotator SocketRotation = GetMesh()->GetSocketRotation(SocketName);

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		Effect,
		SocketLocation,
		SocketRotation
	);
}

// STATS
int32 AMainCharacter::GetHealth() {
	return Health;
}

void AMainCharacter::SetHealth(int32 NewHealth) {
	Health = FMath::Clamp(NewHealth, 0, MaxHealthUpgraded);
	HealthChangeDelegate.Broadcast(Health);

	if (Health <= 0) {
		PlayerDeathDelegate.Broadcast();
	}
}

int32 AMainCharacter::GetMaxHealth() {
	return MaxHealthUpgraded;
}

void AMainCharacter::SetMaxHealth(int32 NewMaxHealth) {
	MaxHealthUpgraded = FMath::Min(0, NewMaxHealth);
}

// DAMAGEABLE
void AMainCharacter::ReceiveDamage(float RawDamage, EDamageType DamageType) {
	// Cancel damage if invulnerable
	if (bIsInvulnerable) {
		return;
	}

	bIsInvulnerable = true;
	// Invulnerability Cooldown
	GetWorld()->GetTimerManager().SetTimer(
		InvulnerableTimer,
		[this]() { bIsInvulnerable = false; },
		InvulnerableCooldown,
		false
	);

	// Calculate the actual damage
	float ReducedDamage = RawDamage;
	switch (DamageType) {
		case EDamageType::Physical:
			ReducedDamage *= (1.f - PhysicalResistanceInitial);
			break;

		case EDamageType::Magic:
			ReducedDamage *= (1.f - MagicResistanceInitial);
			break;

		default:
			ReducedDamage *= 1;
			break;
	}
	int32 Damage = FMath::RoundToInt(ReducedDamage);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		FString::Printf(TEXT("%f / %f -> %i"), RawDamage, ReducedDamage, Damage)
	);

	// Play Sound depending on damage and health
	float DamagePercentage = Damage > 0.f ? Damage / static_cast<float>(MaxHealthUpgraded) : 0.f;
	float HealthPercentage = Health > 0.f ? Health / static_cast<float>(MaxHealthUpgraded) : 0.f;
	if (DamagePercentage > 0.15f || HealthPercentage <= 0.25f) {
		PlayVoiceAudio(HeavyPainSound);
	}
	else {
		PlayVoiceAudio(PainSound);
	}

	// Play Animation
	PlayAnimMontage(HitAnimMontage, 1.f);

	// Play FX
	PlayParticleSystem(HitFX, StatusSocket);

	// Set the new health
	SetHealth(Health - Damage);

	// Reset Attack State
	ResetBasicAttack();
}

// INTERACTION
void AMainCharacter::UpdateFocusInteractable() {
	if (NearbyInteractables.Num() == 0) {
		ClosestInteractable = nullptr;
		InteractFocusChangeDelegate.Broadcast(nullptr);
		return;
	}

	AActor* Closest = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max(); // Initialize to Max value of float

	for (AActor* Actor : NearbyInteractables) {
		if (!IsValid(Actor)) continue;

		float Distance = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());
		if (Distance < ClosestDistance) {
			Closest = Actor;
			ClosestDistance = Distance;
		}
	}

	if (Closest && Closest->Implements<UInteractable>()) {
		if (ClosestInteractable) {
			if (Closest == ClosestInteractable) return; // No change
			IInteractable::Execute_OnUnfocused(ClosestInteractable); // Previous Closest Interactable is no more
		}

		IInteractable::Execute_OnFocused(Closest);
		ClosestInteractable = Closest;

		InteractFocusChangeDelegate.Broadcast(Closest);
	}
}

// INPUT ACTIONS
void AMainCharacter::Move(const FInputActionValue& Value) {
	if (bIsChargingSpecialAttack) return;

	FVector2D Input = Value.Get<FVector2D>();
	LastMoveInput = Input;

	AddMovementInput(GetActorForwardVector(), Input.Y);
	AddMovementInput(GetActorRightVector(), Input.X);
}

void AMainCharacter::ResetMoveDirection(const FInputActionValue& Value) {
	LastMoveInput = FVector2D(0, 1);
}

void AMainCharacter::Look(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void AMainCharacter::Interact(const FInputActionValue& Value) {
	if (NearbyInteractables.Num() == 0) return;

	if (ClosestInteractable && ClosestInteractable->Implements<UInteractable>()) {
		IInteractable::Execute_Interact(ClosestInteractable, this);
	}
}

void AMainCharacter::Dodge(const FInputActionValue& Value) {
	if (!bCanDodge) {
		PlayVoiceAudio(CooldownVoiceLine);
		return;
	}

	bCanDodge = false;
	// Invulnerability Cooldown
	bIsInvulnerable = true;
	GetWorld()->GetTimerManager().SetTimer(
		InvulnerableTimer,
		[this]() { bIsInvulnerable = false; },
		InvulnerableCooldown,
		false
	);

	// Dodge Direction
	const FVector DodgeDirection = (
		GetActorUpVector() * 0.1f
		+ GetActorForwardVector() * LastMoveInput.Y
		+ GetActorRightVector() * LastMoveInput.X
	).GetSafeNormal();

	// Dodge Launch
	LaunchCharacter(DodgeDirection * DodgeForce, true, true);

	// Cooldown
	GetWorld()->GetTimerManager().SetTimer(
		DodgeTimer,
		[this]() {
			bCanDodge = true;
			bIsInvulnerable = false;
		},
		DodgeCooldown,
		false
	);
}

void AMainCharacter::Shoot(const FInputActionValue& Value) {
	// If attacking, queue the attack
	if (bIsAttacking) {
		bHasQueuedAttack = true;
		return;
	}

	// Dispatch Shoot event if start attacking
	bIsAttacking = true;
	BasicAttack();
}

void AMainCharacter::SpecialAttackStart(const FInputActionValue& Value) {
	if (bIsChargingSpecialAttack) return;

	float LightLeftInSeconds = GameState->GetLightLeftInSeconds();
	if (SpecialAttackLightCost > LightLeftInSeconds) {
		PlayVoiceAudio(OutOfManaVoiceLine);
		return;
	}
	if (!bCanSpecialAttack) {
		PlayVoiceAudio(CooldownVoiceLine);
		return;
	}
	if (bIsAttacking) {
		PlayVoiceAudio(NegationVoiceLine);
		return;
	}

	// Initial cost
	GameState->AddLightInSeconds(SpecialAttackLightCost);

	// Play Start Animation
	PlayAnimMontage(SpecialAttackStartAnimMontage);

	// Play Start Particles
	PlayParticleSystem(MarkTargetingUpperFX, ArmTrailSocket);
	PlayParticleSystem(MarkTargetingLowerFX, HandTrailSocket);

	bIsAttacking = true;
	bCanSpecialAttack = false;
	bIsChargingSpecialAttack = true;

	GetWorld()->GetTimerManager().SetTimer(
		SpecialAttackHoldTimer,
		this,
		&AMainCharacter::OnHoldSpecialAttack,
		HoldUpdateRate,
		true
	);
}

void AMainCharacter::OnHoldSpecialAttack() {
	float LightLeftInSeconds = GameState->GetLightLeftInSeconds();
	if (LightLeftInSeconds < HoldUpdateRate) {
		GetWorld()->GetTimerManager().ClearTimer(SpecialAttackHoldTimer);
		PlayAnimMontage(SpecialAttackEndAnimMontage);
		PlayVoiceAudio(OutOfManaVoiceLine);
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (
		AnimInstance
		&& !AnimInstance->Montage_IsPlaying(SpecialAttackStartAnimMontage)
		&& !AnimInstance->Montage_IsPlaying(SpecialAttackHoldAnimMontage)
	) {
		PlayAnimMontage(SpecialAttackHoldAnimMontage);
	}

	SpecialAttackCurrentCharge += HoldUpdateRate;
	GameState->AddLightInSeconds(-2 * HoldUpdateRate);
}

void AMainCharacter::SpecialAttackRelease(const FInputActionValue& Value) {
	if (!bIsChargingSpecialAttack) return;
	GetWorld()->GetTimerManager().ClearTimer(SpecialAttackHoldTimer);

	float LightLeftInSeconds = GameState->GetLightLeftInSeconds();
	if (LightLeftInSeconds <= 0) {
		PlayVoiceAudio(OutOfManaVoiceLine);
		return;
	}

	PlayParticleSystem(MarkCastFX, HandMuzzle_Socket);
	PlaySoundEffect(ShootSound, HandMuzzle_Socket);
	PlayAnimMontage(SpecialAttackCastAnimMontage, 1.f);

	if (!SpecialAttackProjectile) return;

	// Origin Socket
	FVector ProjectileSpawnLocation = GetMesh()->GetSocketLocation(MarkStart_Socket);
	FRotator ProjectileSpawnRotation = GetMesh()->GetSocketRotation(MarkStart_Socket);

	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ASpecialAttackProjectile* Projectile = GetWorld()->SpawnActor<ASpecialAttackProjectile>(
		SpecialAttackProjectile,
		ProjectileSpawnLocation,
		ProjectileSpawnRotation,
		SpawnParams
	);

	if (Projectile) {
		Projectile->ProjectileDamage = (SpecialAttackUpgradedDamage + ((SpecialAttackUpgradedDamage / 10.f) *
			SpecialAttackCurrentCharge));
		// Projectile->FireProjectile(ProjectileSpawnRotation.Vector());
		Projectile->FireProjectile(ProjectileSpawnRotation.Add(0.f, 90.f, 0.f).Vector());
	}

	// Special Attack Cooldown
	bIsChargingSpecialAttack = false;
	GetWorld()->GetTimerManager().SetTimer(
		SpecialAttackCooldownTimer,
		[this]() {
			bCanSpecialAttack = true;
			bIsAttacking = false;
		},
		SpecialAttackCooldown,
		false
	);
}

// MAIN ATTACK
void AMainCharacter::BasicAttack() {
	AttackCount = (AttackCount + 1) % MaxAttackCount;

	PlayParticleSystem(MuzzleFlashFX, GunBarrel_Socket);
	PlayParticleSystem(MuzzleFX, GunMuzzle_Socket);
	PlaySoundEffect(ShootSound, GunBarrel_Socket);
	PlayAnimMontage(ShootAnimMontage, 1.f);

	if (!BasicAttackProjectile) return;

	// Origin Socket
	FVector ProjectileSpawnLocation = GetMesh()->GetSocketLocation(GunBarrel_Socket);
	FRotator ProjectileSpawnRotation = GetMesh()->GetSocketRotation(GunMuzzle_Socket);

	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ABasicAttackProjectile* Projectile = GetWorld()->SpawnActor<ABasicAttackProjectile>(
		BasicAttackProjectile,
		ProjectileSpawnLocation,
		ProjectileSpawnRotation,
		SpawnParams
	);

	if (Projectile) {
		Projectile->ProjectileDamage = 10.f;
		Projectile->FireProjectile(ProjectileSpawnRotation.Vector());
	}
}

void AMainCharacter::BasicAttackQueue() {
	if (bHasQueuedAttack && !bIsAttacking) {
		bHasQueuedAttack = false;
		BasicAttack();
	}
}

void AMainCharacter::ResetBasicAttack() {
	AttackCount = 0;
	bHasQueuedAttack = false;
	bIsAttacking = false;
}


/*
 *	EVENTS
 */
// DEATH
void AMainCharacter::OnDeath() {
	bIsDead = true;
	bIsInvulnerable = true;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) {
		return;
	}
	DisableInput(PC);

	FVector DeathLocation = GetActorLocation();
	FVector CameraLocation = DeathLocation + CameraOffset;
	FRotator CameraRotation = (DeathLocation - CameraLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACameraActor* SpectatorCamera = GetWorld()->SpawnActor<ACameraActor>(
		ACameraActor::StaticClass(),
		CameraLocation,
		CameraRotation,
		SpawnParams
	);

	if (SpectatorCamera) {
		PC->SetViewTargetWithBlend(SpectatorCamera, 1.0f);
	}

	PlayVoiceAudio(DeathVoiceLine);
	PlayAnimMontage(DeathAnimMontage, 1.f);

	GetWorld()->GetTimerManager().SetTimer(
		DeathTimer,
		[this] {
			GameInstance->LoadLevelByType(ELevelType::SafeArea);
		},
		DeathTimeBeforeLevelLoad,
		false
	);
}

// LIGHT
void AMainCharacter::OnLuminosityChange(float LuminosityPercentage) {
	CharacterSpotlight->SetInnerConeAngle(LuminosityPercentage * LightInnerAngleMaximum);
	CharacterSpotlight->SetOuterConeAngle(
		LuminosityPercentage > 0.f ? LuminosityPercentage * LightInnerAngleMaximum + LightOuterAngleOffset : 0.f
	);
}

// INTERACTION
void AMainCharacter::OnInteractableEnter(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (OtherActor && OtherActor->Implements<UInteractable>()) {
		NearbyInteractables.AddUnique(OtherActor);
	}
}

void AMainCharacter::OnInteractableExit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
	if (OtherActor) {
		NearbyInteractables.Remove(OtherActor);
		if (OtherActor == ClosestInteractable) {
			IInteractable::Execute_OnUnfocused(ClosestInteractable);
			InteractFocusChangeDelegate.Broadcast(nullptr);
			ClosestInteractable = nullptr;
		}
	}
}
