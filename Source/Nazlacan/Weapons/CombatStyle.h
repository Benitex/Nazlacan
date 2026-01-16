#pragma once

#include "CoreMinimal.h"
#include "CombatStyle.generated.h"

UENUM(BlueprintType)
enum class ECombatStyle : uint8 {
    DualWielding,
    TwoHanded,
    TwoSpells,
    SwordAndSorcery,
    Archery,
};
