#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Nazlacan/Systems/Corruption/Sun.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
    None = 0,
    Sword,
    Heavy,
    Bow,
    SpellFocus,
};

USTRUCT(BlueprintType)
struct NAZLACAN_API FWeaponData : public FTableRowBase {
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BaseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AttackSpeedMultiplier = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CriticalDamageMultiplier = 2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESun DefaultSun = ESun::None;
};
