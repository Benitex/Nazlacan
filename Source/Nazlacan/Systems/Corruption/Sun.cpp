#include "Sun.h"

ESun GetRandomSun() {
    const UEnum* SunEnum = StaticEnum<ESun>();
    if (!SunEnum) return ESun::Tezca;

    const int32 RandomIndex = FMath::RandRange(0, SunEnum->NumEnums() - 2);
    return static_cast<ESun>(SunEnum->GetValueByIndex(RandomIndex));
}

ESun GetSunWeakness(ESun Sun) {
    const UEnum* SunEnum = StaticEnum<ESun>();
    uint8 Weakness = static_cast<uint8>(Sun) + 1;
    Weakness %= SunEnum->NumEnums() - 2;
    if (Weakness == 0) Weakness = 1;
    return static_cast<ESun>(Weakness);
}
