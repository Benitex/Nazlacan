#pragma once

#include "CoreMinimal.h"
#include "CardinalDirections.generated.h"

UENUM(BlueprintType)
enum class ECardinalDirections : uint8 {
    None = 0,

    North = 1,
    West = 2,
    South = 3,
    East = 4,
};
