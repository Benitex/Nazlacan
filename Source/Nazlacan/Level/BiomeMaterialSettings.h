#pragma once

#include "CoreMinimal.h"
#include "BiomeMaterialSettings.generated.h"

UCLASS()
class NAZLACAN_API UBiomeMaterialSettings : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	// Grass

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	TObjectPtr<UTexture2D> GrassColorMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	TObjectPtr<UTexture2D> GrassNormalMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	TObjectPtr<UTexture2D> GrassOcclusionRoughnessMetallicMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	FLinearColor GrassTint = FColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	float GrassTextureSize = 2000;

	// Rock

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	TObjectPtr<UTexture2D> RockColorMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	TObjectPtr<UTexture2D> RockNormalMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	TObjectPtr<UTexture2D> RockOcclusionRoughnessMetallicMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	FLinearColor RockTint = FColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	float RockTextureSize = 2000;

	// The "angle" to start creating rocks. Based on the normal of the surface.
	// Negative values will create rocks on surfaces facing downwards, while positive values will create rocks on surfaces facing upwards.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
    float RockBlendBias;

	// Intensity of the transition between rocks and other textures
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
    float RockBlendSharpness;

	// Snow

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	TObjectPtr<UTexture2D> SnowColorMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	TObjectPtr<UTexture2D> SnowNormalMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	TObjectPtr<UTexture2D> SnowOcclusionRoughnessMetallicMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	FLinearColor SnowTint = FColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	float SnowTextureSize = 2000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	float SnowBlendHeight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Snow")
	float SnowBlendSharpness = 10000;
};
