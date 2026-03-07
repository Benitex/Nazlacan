#include "CustomGameState.h"

void ACustomGameState::BeginPlay() {
    Super::BeginPlay();
    RandomStream.GenerateNewSeed();
    if (OnGameSeedReady.IsBound()) OnGameSeedReady.Broadcast();
}

float ACustomGameState::GetSunCorruption(ESun Sun) const {
    // TODO different corruptions according to the percentage of use of the sun
    return GetWorldCorruption();
}

UBiome* ACustomGameState::GetBiomeAt(const FVector& Point) const {
    if (Point.X > 0 && Point.Y > 0) {
        return *Biomes.Find(ECardinalDirections::West);
    } else if (Point.X > 0 && Point.Y < 0) {
        return *Biomes.Find(ECardinalDirections::South);
    } else if (Point.X < 0 && Point.Y > 0) {
        return *Biomes.Find(ECardinalDirections::North);
    } else {
        return *Biomes.Find(ECardinalDirections::East);
    }
}
