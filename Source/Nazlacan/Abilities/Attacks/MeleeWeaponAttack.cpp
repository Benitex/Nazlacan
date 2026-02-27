#include "MeleeWeaponAttack.h"
#include "Nazlacan/Macros.h"

void UMeleeWeaponAttack::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    Super::OnAvatarSet(ActorInfo, Spec);
    MainCharacter = Cast<AMainCharacter>(Character.Get());
}

void UMeleeWeaponAttack::TryToActivateNextAttack() {
    if (!MainCharacter.IsValid()) return;
    MainCharacter->TryToActivateNextAttack();
}

void UMeleeWeaponAttack::StartHitDetection() const {
    if (!MainCharacter.IsValid()) return;
    if (!ensure(bUsesRightHandWeapon || bUsesLeftHandWeapon)) return;

    if (bUsesRightHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetDamageEffectSpecHandleForHand(EEquipmentSlot::RightHand);
        AWeapon* Weapon = GetEquippedWeapon(EEquipmentSlot::RightHand);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
    if (bUsesLeftHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetDamageEffectSpecHandleForHand(EEquipmentSlot::LeftHand);
        AWeapon* Weapon = GetEquippedWeapon(EEquipmentSlot::LeftHand);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
}

FGameplayEffectSpecHandle UMeleeWeaponAttack::GetDamageEffectSpecHandleForHand(const EEquipmentSlot Hand) const {
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApplyOnHit, GetAbilityLevel());
    returnIfNull(MainCharacter.Get()) SpecHandle;
    AWeapon* EquippedWeapon = GetEquippedWeapon(Hand);
    returnIfNull(EquippedWeapon) SpecHandle;

    const float Damage = EquippedWeapon->GetWeaponData().BaseDamage * MotionValue;
    const float CriticalDamageMultiplier = EquippedWeapon->GetWeaponData().CriticalDamageMultiplier;
    ESun Sun = ESun::None;
    if (bShouldApplyElementalDamage) Sun = EquippedWeapon->GetDominantSun();

    return GetDamageEffectSpecHandle(Damage, Sun, CriticalDamageMultiplier, EquippedWeapon);
}

void UMeleeWeaponAttack::StopHitDetection() const {
    if (!MainCharacter.IsValid()) return;

    if (bUsesRightHandWeapon) {
        const AWeapon* Weapon = GetEquippedWeapon(EEquipmentSlot::RightHand);
        if (IsValid(Weapon)) Weapon->StopCollisionDetection();
    }
    if (bUsesLeftHandWeapon) {
        const AWeapon* Weapon = GetEquippedWeapon(EEquipmentSlot::LeftHand);
        if (IsValid(Weapon)) Weapon->StopCollisionDetection();
    }
}

bool UMeleeWeaponAttack::ShouldMoveDuringAttack() const {
    if (!MainCharacter.IsValid()) return false;
    if (MainCharacter->GetMovementIntendedDirection().IsNearlyZero()) return false;
    return Super::ShouldMoveDuringAttack();
}

void UMeleeWeaponAttack::RemoveLastAttackTag() const {
    if (MainCharacter.IsValid()) MainCharacter->RemoveLastAttack();
}

AWeapon* UMeleeWeaponAttack::GetEquippedWeapon(const EEquipmentSlot InHand) const {
    if (!MainCharacter.IsValid()) return nullptr;
    return MainCharacter->GetState()->GetEquippedWeapon(InHand);
}
