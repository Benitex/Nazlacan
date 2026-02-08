#pragma once

#include "CoreMinimal.h"
#include "BiomeMaterialSettings.h"
#include "GameplayEffect.h"
#include "Nazlacan/Systems/Corruption/Sun.h"
#include "Biome.generated.h"

UCLASS(BlueprintType)
class NAZLACAN_API UBiome : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	// Magnitude of the noise. Higher values result in taller mountains.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float MountainsHeight;

	// Frequency of the noise. Lower values result in more spread-out mountains.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float MountainsDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	int NoiseLayersCount = 3;

	// The reduction of amplitude applied to each successive layer of noise.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain", meta = (ClampMin = "1.0"))
	float MountainsReductionDivisor = 2;

	// The factor by which to multiply the base density of mountains in each layer of noise.
	// Higher values result in more detailed rocks and cliffs.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	float MountainsDensityFactor = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Terrain")
	TObjectPtr<UBiomeMaterialSettings> MaterialSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Corruption")
	ESun DominantSun = ESun::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PCG Biome Core")
	TObjectPtr<UPrimaryDataAsset> BiomeDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PCG Biome Core")
    TObjectPtr<UPrimaryDataAsset> BiomeTemplate;

    // TODO pcg graph

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply = {};

	// TODO enemies list
};
