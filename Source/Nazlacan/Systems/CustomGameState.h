#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameState.generated.h"

UCLASS()
class NAZLACAN_API ACustomGameState : public AGameStateBase {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Corruption")
	float WorldCorruption;

	// How much the world corruption increases per second
	UPROPERTY(EditDefaultsOnly, Category = "Corruption")
	float CorruptionAccelerationRate;

public:
	float GetWorldCorruption() const { return WorldCorruption; }
};
