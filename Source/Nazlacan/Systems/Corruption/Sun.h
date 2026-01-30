#pragma once

#include "CoreMinimal.h"
#include "Sun.generated.h"

UENUM(BlueprintType)
enum class ESun : uint8 {
    None = 0,
    Tezca = 1,
    Quetza = 2,
    Tlaloc = 3,
    Chal = 4,
};

ESun GetRandomSun();
ESun GetSunWeakness(ESun Sun);
