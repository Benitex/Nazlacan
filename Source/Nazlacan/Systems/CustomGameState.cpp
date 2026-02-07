#include "CustomGameState.h"

void ACustomGameState::BeginPlay() {
    Super::BeginPlay();
    RandomStream.GenerateNewSeed();
    if (OnGameSeedReady.IsBound()) OnGameSeedReady.Broadcast();
}
