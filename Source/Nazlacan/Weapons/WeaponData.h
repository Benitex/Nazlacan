#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
    Sword,
    Greatsword,
    Bow,
    Spell,
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
};
