#pragma once

#include "CoreMinimal.h"
#include "BiomeMaterialSettings.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "Nazlacan/Systems/Corruption/Sun.h"
#include "Biome.generated.h"

UCLASS(BlueprintType)
class NAZLACAN_API UBiome : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	// Magnitude of the noise. Higher values result in taller mountains.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
	float MountainsHeight;

	// Frequency of the noise. Lower values result in more spread-out mountains.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
	float MountainsDensity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain", meta = (ClampMin = "1.0"))
	int NoiseLayersCount = 3;

	// The reduction of amplitude applied to each successive layer of noise.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
	float MountainsReductionDivisor = 2;

	// The factor by which to multiply the base density of mountains in each layer of noise.
	// Higher values result in more detailed rocks and cliffs.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
	float MountainsDensityFactor = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
    float WaterHeightLevel = -20;

	// Applies an offset to the position of the terrain, which can be used to make it higher or lower by default.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
    float BiomeWorldHeight = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Terrain")
	TObjectPtr<UBiomeMaterialSettings> MaterialSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
	TArray<UNiagaraSystem*> VisualEffects = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Corruption")
	ESun DominantSun = ESun::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply = {};

	// TODO enemies list

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PCG Biome Core")
	TObjectPtr<UPrimaryDataAsset> BiomeDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PCG Biome Core")
    TArray<UPrimaryDataAsset*> BiomeAssets;
};
