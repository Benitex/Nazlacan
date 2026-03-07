#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Nazlacan/Level/Biome.h"
#include "Nazlacan/Level/CardinalDirections.h"
#include "CustomGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameSeedReady);

UCLASS()
class NAZLACAN_API ACustomGameState : public AGameStateBase {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Corruption")
	float WorldCorruption;

	// How much the world corruption increases per second
	UPROPERTY(EditDefaultsOnly, Category = "Corruption")
	float CorruptionAccelerationRate;

	UPROPERTY(BlueprintAssignable)
	FOnGameSeedReady OnGameSeedReady;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Biomes")
	TMap<ECardinalDirections, TObjectPtr<UBiome>> Biomes = {};

	UPROPERTY(BlueprintReadWrite)
	FRandomStream RandomStream;

public:
	float GetWorldCorruption() const { return WorldCorruption; }
	float GetSunCorruption(ESun Sun) const;

	UFUNCTION(BlueprintCallable)
	void SetBiomeForDirection(UBiome* Biome, const ECardinalDirections Direction) {
		Biomes.Add(Direction, Biome);
	}

	UFUNCTION(BlueprintCallable)
	UBiome* GetBiomeAt(const FVector& Point) const;

protected:
	virtual void BeginPlay() override;
};
