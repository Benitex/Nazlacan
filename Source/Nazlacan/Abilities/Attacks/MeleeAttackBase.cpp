#include "MeleeAttackBase.h"
#include "Nazlacan/Macros.h"

void UMeleeAttackBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    Super::OnAvatarSet(ActorInfo, Spec);
    MainCharacter = Cast<AMainCharacter>(Character.Get());
}

void UMeleeAttackBase::TryToActivateNextAttack() {
    if (!MainCharacter.IsValid()) return;
    MainCharacter->TryToActivateNextAttack();
}

void UMeleeAttackBase::StartHitDetection() const {
    if (!MainCharacter.IsValid()) return;
    if (!ensure(bUsesRightHandWeapon || bUsesLeftHandWeapon)) return;

    if (bUsesRightHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(EEquipmentSlot::RightHand);
        AWeapon* Weapon = GetEquippedWeapon(EEquipmentSlot::RightHand);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
    if (bUsesLeftHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(EEquipmentSlot::LeftHand);
        AWeapon* Weapon = GetEquippedWeapon(EEquipmentSlot::LeftHand);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
}

FGameplayEffectSpecHandle UMeleeAttackBase::GetEffectSpecHandle(const EEquipmentSlot ForHand) const {
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApplyOnHit, GetAbilityLevel());
    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
    returnIfNull(MainCharacter.Get()) SpecHandle;

    AWeapon* EquippedWeapon = GetEquippedWeapon(ForHand);
    Spec->GetContext().AddInstigator(GetAvatarActorFromActorInfo(), IsValid(EquippedWeapon) ? EquippedWeapon : GetAvatarActorFromActorInfo());
    returnIfNull(EquippedWeapon) SpecHandle;

    float WeaponDamage = EquippedWeapon->GetWeaponData().BaseDamage;
    WeaponDamage *= MotionValue;
    static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    Spec->SetSetByCallerMagnitude(DamageTag, WeaponDamage);

    ESun Sun = ESun::None;
    if (bShouldApplyElementalDamage) Sun = EquippedWeapon->GetDominantSun();
    static const FGameplayTag SunTag = FGameplayTag::RequestGameplayTag(FName("Data.Sun"));
    Spec->SetSetByCallerMagnitude(SunTag, static_cast<float>(Sun));

    static const FGameplayTag CriticalTag = FGameplayTag::RequestGameplayTag(FName("Data.CriticalMultiplier"));
    Spec->SetSetByCallerMagnitude(CriticalTag, EquippedWeapon->GetWeaponData().CriticalMultiplier);

    return SpecHandle;
}

void UMeleeAttackBase::StopHitDetection() const {
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

bool UMeleeAttackBase::ShouldMoveDuringAttack() const {
    if (SpeedModifier == 0) return false;

    if (!MainCharacter.IsValid()) return false;
    if (MainCharacter->GetMovementIntendedDirection().IsNearlyZero()) return false;

    return true;
}

void UMeleeAttackBase::RemoveLastAttackTag() const {
    if (MainCharacter.IsValid()) MainCharacter->RemoveLastAttack();
}

AWeapon* UMeleeAttackBase::GetEquippedWeapon(const EEquipmentSlot InHand) const {
    if (!MainCharacter.IsValid()) return nullptr;
    return MainCharacter->GetState()->GetEquippedWeapon(InHand);
}
