#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
    Sword,
    Heavy,
    Bow,
    SpellFocus,
    None = 0,
};

USTRUCT(BlueprintType)
struct NAZLACAN_API FWeaponData : public FTableRowBase {
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BaseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AttackSpeedMultiplier = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CriticalMultiplier = 1;
};
