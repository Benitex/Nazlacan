#pragma once

#include "CoreMinimal.h"
#include "Sun.generated.h"

UENUM(BlueprintType)
enum class ESun : uint8 {
    Tezca = 0,
    Quetza = 1,
    Tlaloc = 2,
    Chal = 3,
};

ESun GetRandomSun();
ESun GetSunWeakness(ESun Sun);
