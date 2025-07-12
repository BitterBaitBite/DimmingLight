#include "InteractableChest.h"

#include "ECombatUpgradeType.h"
#include "MainGameInstance.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "Components/RectLightComponent.h"
#include "Logging/StructuredLog.h"


AInteractableChest::AInteractableChest() {
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	LidPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Lid Pivot"));
	LidPivot->SetupAttachment(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lid Mesh"));
	LidMesh->SetupAttachment(LidPivot);

	ChestLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Chest Light"));
	ChestLight->SetupAttachment(RootComponent);
	ChestLight->Intensity = 0.f;
}

void AInteractableChest::BeginPlay() {
	Super::BeginPlay();

	ChestLight->Intensity = 0.f;

	GameInstance = Cast<UMainGameInstance>(GetGameInstance());
}

void AInteractableChest::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!bIsOpen) return;

	if (ChestLight->Intensity < OpenLightIntensity) {
		ChestLight->Intensity = FMath::InterpEaseIn(0.f, OpenLightIntensity, (ChestLight->Intensity / OpenLightIntensity + DeltaTime),
		                                            IntensityChangeSpeed);
		// ChestLight->Intensity += IntensityChangeSpeed * DeltaTime;
	}
	// else {
	// 	ChestLight->Intensity = OpenLightIntensity;
	// }

	FRotator CurrentLidRotation = LidPivot->GetRelativeRotation();
	if (CurrentLidRotation.Pitch < OpenLidRotation.Pitch) {
		float PitchDelta = OpenLidSpeed * DeltaTime;
		FRotator NewLidRotation = CurrentLidRotation + FRotator(PitchDelta, 0.f, 0.f);
		NewLidRotation.Pitch = FMath::InterpEaseIn(0., OpenLidRotation.Pitch,
		                                           (CurrentLidRotation.Pitch / OpenLidRotation.Pitch + DeltaTime), OpenLidSpeed);

		LidPivot->SetRelativeRotation(NewLidRotation);
	}
	else {
		LidPivot->SetRelativeRotation(OpenLidRotation);
	}
}

void AInteractableChest::Interact_Implementation(AActor* InteractionActor) {
	IInteractable::Interact_Implementation(InteractionActor);

	if (!bIsOpen) {
		OpenChest();
	}
}

FText AInteractableChest::GetInteraction_Implementation() {
	if (bIsOpen) {
		return FText::FromString(TEXT("Already open"));
	}

	return FText::FromString(TEXT("Open chest"));
}

void AInteractableChest::OnFocused_Implementation() {
	BaseMesh->SetRenderCustomDepth(true);
	LidMesh->SetRenderCustomDepth(true);
}

void AInteractableChest::OnUnfocused_Implementation() {
	BaseMesh->SetRenderCustomDepth(false);
	LidMesh->SetRenderCustomDepth(false);
}

void AInteractableChest::OpenChest() {
	bIsOpen = true;

	EChestRewardsType RewardType = ChestRewardsAvailability;
	if (RewardType == EChestRewardsType::UpgradeOrCurrency) {
		RewardType = FMath::RandBool() ? EChestRewardsType::CurrencyOnly : EChestRewardsType::UpgradeOnly;
	}
	switch (RewardType) {
		case EChestRewardsType::CurrencyOnly: {
			float CurrencyReward = FMath::RandRange(MinCurrencyReward, MaxCurrencyReward);
			GameInstance->AddCurrency(CurrencyReward);
			break;
		}

		case EChestRewardsType::UpgradeOnly: {
			FUpgradeData CurrentUpgrades = GameInstance->GetUpgradeData();
			for (ECombatUpgradeType Upgrade : AvailableUpgrades) {
				if (!CurrentUpgrades.CombatUpgrades.Contains(Upgrade)) {
					CurrentUpgrades.CombatUpgrades.Add(Upgrade);
				}
			}
			GameInstance->SetUpgradeData(CurrentUpgrades);
			break;
		}

		case EChestRewardsType::UpgradeAndCurrency: {
			float CurrencyReward = FMath::RandRange(MinCurrencyReward, MaxCurrencyReward);
			GameInstance->AddCurrency(CurrencyReward);

			FUpgradeData CurrentUpgrades = GameInstance->GetUpgradeData();
			for (ECombatUpgradeType Upgrade : AvailableUpgrades) {
				if (!CurrentUpgrades.CombatUpgrades.Contains(Upgrade)) {
					CurrentUpgrades.CombatUpgrades.Add(Upgrade);
				}
			}
			GameInstance->SetUpgradeData(CurrentUpgrades);
		}

		default:
			UE_LOGFMT(LogTemp, Error, "AInteractableChest Error | OpenChest: The specified reward type is not contemplated");
			break;
	}
}
