#include "MeleeAttack.h"
#include "Nazlacan/Characters/Enemies/Enemy.h"

void UMeleeAttack::StartHitDetection() const {
    ESun Sun = ESun::None;
    if (bShouldApplyElementalDamage) Sun = Character->GetDominantSun();

    GetDamageEffectSpecHandle(
        Character->GetBaseDamage(),
        Sun,
        DefaultCriticalDamageMultiplier
    );
    // TODO
}

void UMeleeAttack::StopHitDetection() const {
    // TODO
}

FGameplayEffectSpecHandle UMeleeAttack::GetDamageEffectSpecHandle(
    const float Damage,
    const ESun Sun,
    const float CriticalDamageMultiplier,
    AActor* EffectCauser
) const {
    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectToApplyOnHit, GetAbilityLevel());
    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

    if (!EffectCauser) EffectCauser = GetAvatarActorFromActorInfo();
    Spec->GetContext().AddInstigator(GetAvatarActorFromActorInfo(), EffectCauser);

    static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    Spec->SetSetByCallerMagnitude(DamageTag, Damage);

    static const FGameplayTag SunTag = FGameplayTag::RequestGameplayTag(FName("Data.Sun"));
    Spec->SetSetByCallerMagnitude(SunTag, static_cast<float>(Sun));

    static const FGameplayTag CriticalTag = FGameplayTag::RequestGameplayTag(FName("Data.CriticalMultiplier"));
    Spec->SetSetByCallerMagnitude(CriticalTag, CriticalDamageMultiplier);

    return SpecHandle;
}

bool UMeleeAttack::ShouldMoveDuringAttack() const {
    if (SpeedModifier == 0) return false;
    return true;
}
