#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "InputMappingContext.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


class ASpecialAttackProjectile;
class ABasicAttackProjectile;
class AMainGameState;
class UBoxComponent;
class IInteractable;
class UMainGameInstance;
class USpotLightComponent;
class UInputAction;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShoot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, int32, NewHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractFocusChange, AActor*, NewInteractable);


UCLASS()
class DIMMINGLIGHT_API AMainCharacter : public ACharacter, public IDamageable {
	GENERATED_BODY()

	public:
		AMainCharacter();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		// DELEGATES
		UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnDeath PlayerDeathDelegate;
		UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnShoot ShootDelegate;
		UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnHealthChange HealthChangeDelegate;
		UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnInteractFocusChange InteractFocusChangeDelegate;

		// AUDIO
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UAudioComponent> VoiceAudioComponent = nullptr;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> PainSound = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> HeavyPainSound = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> ShootSound = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> NegationVoiceLine = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> CooldownVoiceLine = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> OutOfManaVoiceLine = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		TObjectPtr<USoundBase> DeathVoiceLine = nullptr;

		UFUNCTION(BlueprintCallable, Category = "Audio")
		void PlayVoiceAudio(USoundBase* VoiceAudio);

		UFUNCTION(BlueprintCallable, Category = "Audio")
		void PlaySoundEffect(USoundBase* FXAudio, FName SocketName);

		// EFFECTS
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MuzzleFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MuzzleFlashFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkTrailFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkTargetingUpperFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkTargetingLowerFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkTargetLoopFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkTargetBallFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkCastFX = nullptr;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> MarkEndFX = nullptr;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
		TObjectPtr<UParticleSystem> HitFX = nullptr;

		UFUNCTION(BlueprintCallable, Category = "VFX")
		void PlayParticleSystem(UParticleSystem* Effect, FName SocketName);

		// SOCKETS
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName GunBarrel_Socket = TEXT("FX_Gun_Barrel");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName GunMuzzle_Socket = TEXT("Muzzle_02");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName HandMuzzle_Socket = TEXT("Muzzle_01");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName Head_Socket = TEXT("FX_Head");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName ArmTrailSocket = TEXT("FX_Trail_R_01");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName HandTrailSocket = TEXT("FX_Trail_R_02");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName RootSocket = TEXT("FX_Root");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName MarkStart_Socket = TEXT("Mark_Start");
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sockets")
		FName StatusSocket = TEXT("Status");

		// INPUT
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputMappingContext> DefaultMappingContext;

		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> MoveAction;

		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> LookAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> InteractAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> DodgeAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> ShootAction;
		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> SpecialAction;

		// STATS
		UFUNCTION(BlueprintCallable, Category="Stats")
		int32 GetHealth();
		UFUNCTION(BlueprintCallable, Category="Stats")
		void SetHealth(int32 NewHealth);

		UFUNCTION(BlueprintCallable, Category="Stats")
		int32 GetMaxHealth();
		UFUNCTION(BlueprintCallable, Category="Stats")
		void SetMaxHealth(int32 NewMaxHealth);

		UFUNCTION(BlueprintCallable, Category="Damageable")
		virtual void ReceiveDamage(float RawDamage, EDamageType DamageType) override;

		// LIGHT
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light")
		TObjectPtr<UMeshComponent> LightSourceMesh = nullptr;
		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Light")
		TObjectPtr<USpotLightComponent> CharacterSpotlight = nullptr;
		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Light")
		TObjectPtr<UParticleSystemComponent> LightEffectComponent = nullptr;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
		float LightIntensity = 15000.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
		float LightInnerAngleMaximum = 90.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
		float LightOuterAngleOffset = 15.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
		FColor LightColor = FColor::White;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
		float LightAttenuationRadius = 1000.f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Light")
		float LightTemperature = 1500.f;

		// COMBAT
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
		TSubclassOf<ABasicAttackProjectile> BasicAttackProjectile = nullptr;
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
		TSubclassOf<ASpecialAttackProjectile> SpecialAttackProjectile = nullptr;

		// Basic Attack
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
		float BasicAttackInitialDamage = 10.f;
		UPROPERTY(Transient, SkipSerialization)
		float BasicAttackUpgradedDamage = 10.f;
		UPROPERTY(Transient, SkipSerialization)
		float BasicAttackCurrentDamage = 10.f;

		UFUNCTION(BlueprintCallable, Category="Combat")
		void BasicAttackQueue();
		UFUNCTION(BlueprintCallable, Category="Combat")
		void ResetBasicAttack();
		UFUNCTION(BlueprintCallable, Category="Combat")
		void BasicAttack();

		// Special Attack
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
		float SpecialAttackInitialDamage = 100.f;
		UPROPERTY(Transient, SkipSerialization)
		float SpecialAttackUpgradedDamage = 100.f;
		UPROPERTY(Transient, SkipSerialization)
		float SpecialAttackCurrentDamage = 100.f;
		UPROPERTY(Transient, SkipSerialization)
		float SpecialAttackCurrentCharge = 0.f;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
		float SpecialAttackMaxCharge = 2.5f;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Combat")
		float SpecialAttackCooldown = 5.f;

		UPROPERTY(Transient, SkipSerialization)
		FTimerHandle SpecialAttackHoldTimer;
		UPROPERTY(Transient, SkipSerialization)
		FTimerHandle SpecialAttackCooldownTimer;

		// INTERACTION
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AActor> ClosestInteractable = nullptr;
		UPROPERTY(Transient, SkipSerialization)
		TArray<TObjectPtr<AActor>> NearbyInteractables = {};

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
		TObjectPtr<UBoxComponent> InteractBoxComponent = nullptr;

		UFUNCTION()
		void UpdateFocusInteractable();

		// ANIMATION
		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> ShootAnimMontage = nullptr;

		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> HitAnimMontage = nullptr;

		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> DeathAnimMontage = nullptr;

		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> InteractAnimMontage = nullptr;

		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> SpecialAttackStartAnimMontage = nullptr;
		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> SpecialAttackHoldAnimMontage = nullptr;
		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> SpecialAttackCastAnimMontage = nullptr;
		UPROPERTY(EditAnywhere, Category="Animation")
		TObjectPtr<UAnimMontage> SpecialAttackEndAnimMontage = nullptr;


		// DEATH
		UPROPERTY(Transient, SkipSerialization)
		FTimerHandle DeathTimer;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Death")
		float DeathTimeBeforeLevelLoad = 5.f;

		// Spectator camera on death
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
		FVector CameraOffset = FVector(0.f, -300.f, 150.f);

	private:
		// Game State
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<AMainGameState> GameState = nullptr;

		// Game Instance
		UPROPERTY(Transient, SkipSerialization)
		TObjectPtr<UMainGameInstance> GameInstance = nullptr;

		// Main Attack control properties
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Combat")
		int32 MaxAttackCount = 2;
		int32 AttackCount = 0;
		bool bIsAttacking = false;
		bool bHasQueuedAttack = false;

		// Special Attack control properties
		UPROPERTY(EditAnywhere, NotBlueprintable, Category="Combat")
		float SpecialAttackLightCost = -10.f;
		bool bCanSpecialAttack = true;
		bool bIsChargingSpecialAttack = false;
		UPROPERTY(EditAnywhere, NotBlueprintable, Category="Combat")
		float HoldUpdateRate = 0.016f;

		// Dodge control properties
		bool bCanDodge = true;
		FVector2D LastMoveInput = FVector2D(0.f, 1.f);

		// STATS
		bool bIsDead = false;
		bool bIsInvulnerable = false;

		UPROPERTY(EditAnywhere, NotBlueprintable, Category="Stats")
		int32 Health = 100;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		int32 MaxHealthInitial = 100;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		int32 MaxHealthUpgraded = 100;

		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		float PhysicalResistanceInitial = 0.1f;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		float PhysicalResistanceUpgraded = 0.1f;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		float MaxPhysicResistance = 0.9f;

		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		float MagicResistanceInitial = 0.1f;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		float MagicResistanceUpgraded = 0.1f;
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Stats")
		float MaxMagicResistance = 0.9f;

		// DODGE ABILITY
		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Abilities")
		float DodgeForce = 100.f;

		UPROPERTY(EditDefaultsOnly, NotBlueprintable, Category="Abilities")
		float DodgeCooldown = 5.f;

		UPROPERTY(Transient, SkipSerialization)
		FTimerHandle DodgeTimer;

		// INVULNERABLE
		UPROPERTY(EditAnywhere, NotBlueprintable, Category="Abilities")
		float InvulnerableCooldown = 0.75f;

		UPROPERTY(Transient, SkipSerialization)
		FTimerHandle InvulnerableTimer;

		// INPUT ACTIONS
		UFUNCTION()
		void Move(const FInputActionValue& Value);

		UFUNCTION()
		void ResetMoveDirection(const FInputActionValue& Value);

		UFUNCTION()
		void Look(const FInputActionValue& Value);

		UFUNCTION()
		void Interact(const FInputActionValue& Value);

		UFUNCTION()
		void Dodge(const FInputActionValue& Value);

		UFUNCTION()
		void Shoot(const FInputActionValue& Value);

		UFUNCTION()
		void SpecialAttackStart(const FInputActionValue& Value);
		UFUNCTION()
		void SpecialAttackRelease(const FInputActionValue& Value);

		void OnHoldSpecialAttack();

		// EVENTS
		UFUNCTION()
		void OnDeath();

		UFUNCTION()
		void OnLuminosityChange(float LuminosityPercentage);

		UFUNCTION()
		void OnInteractableEnter(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

		UFUNCTION()
		void OnInteractableExit(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);
};
