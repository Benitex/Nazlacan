#pragma once

#include "CoreMinimal.h"
#include "Nazlacan/Level/CardinalDirections.h"
#include "Sun.generated.h"

UENUM(BlueprintType)
enum class ESun : uint8 {
    None = 0,

    Tezca = 1,
    Quetza = 2,
    Huitzloc = 3,
    ChalTotec = 4,
};

ESun GetRandomSun();
ESun GetSunWeakness(ESun Sun);
ECardinalDirections GetSunCardinalDirection(ESun Sun);
