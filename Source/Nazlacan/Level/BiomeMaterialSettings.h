#pragma once

#include "CoreMinimal.h"
#include "BiomeMaterialSettings.generated.h"

UCLASS()
class NAZLACAN_API UBiomeMaterialSettings : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	TObjectPtr<UTexture2D> GrassColorMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	TObjectPtr<UTexture2D> GrassNormalMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	TObjectPtr<UTexture2D> GrassOcclusionRoughnessMetallicMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grass")
	float GrassTextureSize = 64;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	TObjectPtr<UTexture2D> RockColorMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	TObjectPtr<UTexture2D> RockNormalMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	TObjectPtr<UTexture2D> RockOcclusionRoughnessMetallicMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rock")
	float RockTextureSize = 64;
};
