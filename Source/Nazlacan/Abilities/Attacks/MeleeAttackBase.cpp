#include "MeleeAttackBase.h"
#include "Nazlacan/Characters/MainCharacter.h"

void UMeleeAttackBase::TryToActivateNextAttack() {
    if (MainCharacter.IsValid()) MainCharacter->TryToActivateNextAttack();
}

void UMeleeAttackBase::StartHitDetection() const {
    if (!MainCharacter.IsValid()) return;
    if (!ensure(UsesRightHandWeapon || UsesLeftHandWeapon)) return;

    if (UsesRightHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(ACustomPlayerState::RightHandIndex);
        MainCharacter->GetEquippedWeapon(ACustomPlayerState::RightHandIndex)->StartCollisionDetection(MainCharacter.Get(), SpecHandle);
    }
    if (UsesLeftHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(ACustomPlayerState::LeftHandIndex);
        MainCharacter->GetEquippedWeapon(ACustomPlayerState::LeftHandIndex)->StartCollisionDetection(MainCharacter.Get(), SpecHandle);
    }
}

FGameplayEffectSpecHandle UMeleeAttackBase::GetEffectSpecHandle(const uint8 ForHand) const {
    ensure(MainCharacter.IsValid());

    float WeaponDamage = MainCharacter->GetEquippedWeapon(ForHand)->GetWeaponData().BaseDamage;
    WeaponDamage *= MotionValue;

    static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApplyOnHit, GetAbilityLevel());
    SpecHandle.Data.Get()->SetSetByCallerMagnitude(DamageTag, WeaponDamage);

    return SpecHandle;
}

void UMeleeAttackBase::StopHitDetection() const {
    if (!MainCharacter.IsValid()) return;

    if (UsesRightHandWeapon) {
        MainCharacter->GetEquippedWeapon(ACustomPlayerState::RightHandIndex)->StopCollisionDetection();
    }
    if (UsesLeftHandWeapon) {
        MainCharacter->GetEquippedWeapon(ACustomPlayerState::LeftHandIndex)->StopCollisionDetection();
    }
}

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

void UMeleeAttackBase::RemoveLastAttackTag() const {
    if (MainCharacter.IsValid()) MainCharacter->RemoveLastAttack(GetAssetTags());
}
