#include "MeleeAttackBase.h"
#include "Nazlacan/Characters/MainCharacter.h"

void UMeleeAttackBase::TryToActivateNextAttack() {
    if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get())) {
        MainCharacter->TryToActivateNextAttack();
    }
}

void UMeleeAttackBase::StartHitDetection() const {
    if (!ensure(UsesRightHandWeapon || UsesLeftHandWeapon)) return;

    if (UsesRightHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(ACustomPlayerState::RightHandIndex);
        AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::RightHandIndex);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
    if (UsesLeftHandWeapon) {
        const FGameplayEffectSpecHandle SpecHandle = GetEffectSpecHandle(ACustomPlayerState::LeftHandIndex);
        AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::LeftHandIndex);
        Weapon->StartCollisionDetection(GetAvatarActorFromActorInfo(), SpecHandle);
    }
}

FGameplayEffectSpecHandle UMeleeAttackBase::GetEffectSpecHandle(const uint8 ForHand) const {
    float WeaponDamage = Character->GetEquippedWeapon(ForHand)->GetWeaponData().BaseDamage;
    WeaponDamage *= MotionValue;

    static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApplyOnHit, GetAbilityLevel());
    SpecHandle.Data.Get()->SetSetByCallerMagnitude(DamageTag, -WeaponDamage);

    return SpecHandle;
}

void UMeleeAttackBase::StopHitDetection() const {
    if (!Character.IsValid()) return;

    if (UsesRightHandWeapon) {
        const AWeapon* Weapon = Character->GetEquippedWeapon(ACustomPlayerState::RightHandIndex);
        Weapon->StopCollisionDetection();
    }
    if (UsesLeftHandWeapon) {
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

bool UMeleeAttackBase::ShouldEndAttackEarly() const {
    const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get());
    if (!MainCharacter) return false;
    return MainCharacter->IsNextAttackPrepared();
}

void UMeleeAttackBase::RemoveLastAttackTag() const {
    if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get())) {
        MainCharacter->RemoveLastAttack(GetAssetTags());
    }
}
