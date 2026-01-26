#include "MeleeAttackBase.h"
#include "Nazlacan/Characters/MainCharacter.h"

bool UMeleeAttackBase::ShouldMoveDuringAttack() const {
    if (VelocityModifier == 0) return false;

    if (!MainCharacter.IsValid()) return false;
    if (MainCharacter->GetMovementIntendedDirection().IsNearlyZero()) return false;

    return true;
}

bool UMeleeAttackBase::ShouldEndAttackEarly() const {
    if (!MainCharacter.IsValid()) return false;
    return MainCharacter->IsNextAttackPrepared();
}

void UMeleeAttackBase::TryToActivateNextAttack() {
    if (MainCharacter.IsValid()) MainCharacter->TryToActivateNextAttack();
}

void UMeleeAttackBase::RemoveLastAttackTag() const {
    if (MainCharacter.IsValid()) MainCharacter->RemoveLastAttack(GetAssetTags());
}
