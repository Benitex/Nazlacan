#include "MeleeAttackBase.h"
#include "Nazlacan/Characters/PlayerCharacters/MainCharacter.h"

void UMeleeAttackBase::TryToActivateNextAttack() {
    if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get())) {
        MainCharacter->TryToActivateNextAttack();
    }
}

void UMeleeAttackBase::StartHitDetection() const {
    if (!ensure(bUsesRightHandWeapon || bUsesLeftHandWeapon)) return;

    if (bUsesRightHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(ACustomPlayerState::RightHandIndex);
        AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::RightHandIndex);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
    if (bUsesLeftHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(ACustomPlayerState::LeftHandIndex);
        AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::LeftHandIndex);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
}

FGameplayEffectSpecHandle UMeleeAttackBase::GetEffectSpecHandle(const uint8 ForHand) const {
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApplyOnHit, GetAbilityLevel());
    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

    AWeapon* EquippedWeapon = Character->GetEquippedWeapon(ForHand);
    Spec->GetContext().AddInstigator(GetAvatarActorFromActorInfo(), EquippedWeapon ? EquippedWeapon : GetAvatarActorFromActorInfo());

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
    if (!Character.IsValid()) return;

    if (bUsesRightHandWeapon) {
        const AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::RightHandIndex);
        Weapon->StopCollisionDetection();
    }
    if (bUsesLeftHandWeapon) {
        const AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::LeftHandIndex);
        Weapon->StopCollisionDetection();
    }
}

bool UMeleeAttackBase::ShouldMoveDuringAttack() const {
    if (SpeedModifier == 0) return false;

    const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get());
    if (!MainCharacter) return false;
    if (MainCharacter->GetMovementIntendedDirection().IsNearlyZero()) return false;

    return true;
}

void UMeleeAttackBase::RemoveLastAttackTag() const {
    if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get())) {
        MainCharacter->RemoveLastAttack();
    }
}
