#include "ConsumeSkull.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/MainCharacter.h"

void UConsumeSkull::ConsumeSkull() const {
    AActor* Actor = GetAvatarActorFromActorInfo();
    const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Actor);
    returnIfNull(MainCharacter);

    const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(HealGameplayEffect, GetAbilityLevel());
    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

    static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Data.Heal"));
    Spec->SetSetByCallerMagnitude(Tag, MainCharacter->GetDefaultHealingPower());

    MainCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
}

bool UConsumeSkull::ShouldHeal() const {
    AActor* Actor = GetAvatarActorFromActorInfo();
    const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Actor);
    returnIfNull(MainCharacter) false;

    return !MainCharacter->GetState()->HasFullHealth();
}
