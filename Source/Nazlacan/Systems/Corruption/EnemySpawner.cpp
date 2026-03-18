#include "EnemySpawner.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/Enemies/Enemy.h"

void UEnemySpawner::BeginPlay() {
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<ACustomGameState>();

	RaritySpawnChances.ValueSort([](const float Rarity1, const float Rarity2) {
		return Rarity1 < Rarity2;
	});

	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimer,
		this,
		&UEnemySpawner::SpawnEnemies,
		SpawningIntervalSeconds,
		true
	);
}

void UEnemySpawner::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

void UEnemySpawner::SpawnEnemies() const {
	const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	returnIfNull(NavigationSystem);
	if (!GameState.IsValid()) return;

	for (int EnemyNumber = 0; EnemyNumber < GetNumberOfEnemiesToSpawn(); ++EnemyNumber) {
		FNavLocation SpawnLocation;
		NavigationSystem->GetRandomReachablePointInRadius(GetOwner()->GetActorLocation(), SpawnRadius, SpawnLocation);
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, GetOwner()->GetActorLocation());

		const FTransform EnemyPosition = FTransform(Rotation, SpawnLocation.Location);
		const FEnemyData* EnemyData = GetRandomEnemyData(SpawnLocation.Location).GetPtrOrNull();
		if (!EnemyData) continue;
		const float EnemyCorruption = GameState->GetSunCorruption(EnemyData->Sun);

		AEnemy::Spawn(*EnemyData, EnemyCorruption, EnemyPosition, GetWorld());
	}
}

int32 UEnemySpawner::GetNumberOfEnemiesToSpawn() const {
	// TODO add a curve to increase the number of enemies according to the world corruption
	return 3;
}

// ReSharper disable once CppPassValueParameterByConstReference
TOptional<FEnemyData> UEnemySpawner::GetRandomEnemyData(FVector Point) const {
	const UBiome* Biome = GameState->GetBiomeAt(Point);

	// TODO consider enemy rarity

	if (Biome->Enemies.Num() == 0) return TOptional<FEnemyData>();
	return LoadDataFromDataTable(getRandomItemFromArray(Biome->Enemies));
}

TOptional<FEnemyData> UEnemySpawner::LoadDataFromDataTable(const FDataTableRowHandle& DataTableRowHandle) const {
	if (!IsValid(DataTableRowHandle.DataTable) || DataTableRowHandle.RowName.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("Invalid DataTableRowHandle provided for enemy spawning. Skipping enemy spawn."));
		return TOptional<FEnemyData>();
	}

	static const FString ContextString(TEXT("Spawning Enemy"));
	const FEnemyData* EnemyData = DataTableRowHandle.DataTable->FindRow<FEnemyData>(DataTableRowHandle.RowName, ContextString);

	if (!EnemyData) return TOptional<FEnemyData>();
	return *EnemyData;
}
