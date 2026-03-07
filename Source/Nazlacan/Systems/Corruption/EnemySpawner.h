#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Nazlacan/Characters/Enemies/EnemyData.h"
#include "Nazlacan/Systems/CustomGameState.h"
#include "EnemySpawner.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAZLACAN_API UEnemySpawner : public UActorComponent {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpawnRadius = 5000;

	UPROPERTY(EditDefaultsOnly)
	float SpawningIntervalSeconds = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EEnemyRarity, float> RaritySpawnChances = {
		{EEnemyRarity::Common, 0.6},
		{EEnemyRarity::Uncommon, 0.3},
		{EEnemyRarity::Rare, 0.1},
	};

private:
	TWeakObjectPtr<ACustomGameState> GameState;
	FTimerHandle SpawnTimer;

public:	
	UEnemySpawner() = default;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SpawnEnemies() const;
	int32 GetNumberOfEnemiesToSpawn() const;
	TOptional<FEnemyData> GetRandomEnemyData(FVector Point = FVector()) const;

	TOptional<FEnemyData> LoadDataFromDataTable(const FDataTableRowHandle& DataTableRowHandle) const;
};
