#include "CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Nazlacan/Characters/MainCharacter.h"
#include "Net/UnrealNetwork.h"

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
    Super::PreAttributeChange(Attribute, NewValue);
    
    if (Attribute == GetHealthAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
    }
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
        SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));

        if (Health.GetCurrentValue() <= 0) {
            static const FGameplayTagContainer Tag = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Reaction.Death")));
            GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(Tag);
            GrantExperienceToKiller(Data);

        } else if (Data.EvaluatedData.Magnitude < 0) {
            static const FGameplayTagContainer Tag = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Reaction.BeingHit")));
            GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(Tag);
        }
    } else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute()) {
        if (Data.EvaluatedData.Magnitude > 0) {
            SetHealth(GetHealth() + Data.EvaluatedData.Magnitude);
        } else {
            SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
        }
    }
}

void UCharacterAttributeSet::GrantExperienceToKiller(const FGameplayEffectModCallbackData& Data) {
    AActor* Actor = Data.Target.AbilityActorInfo->AvatarActor.Get();
    const IBaseCharacter* DeadCharacter = Cast<IBaseCharacter>(Actor);
    const float BaseExperience = DeadCharacter->GetExperienceDroppedOnDeath();

    const FGameplayEffectContextHandle DeathContext = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* KillerAbilitySystem = DeathContext.GetInstigatorAbilitySystemComponent();
    const AMainCharacter* Killer = Cast<AMainCharacter>(DeathContext.GetInstigator());
    if (!Killer) return;

    FGameplayEffectContextHandle ExperienceEffectContext = KillerAbilitySystem->MakeEffectContext();
    ExperienceEffectContext.AddSourceObject(this);

    const FGameplayEffectSpecHandle EffectSpecHandle = KillerAbilitySystem->MakeOutgoingSpec(
        Killer->GetState()->GetExperienceGrantEffect(),
        1,
        ExperienceEffectContext
    );
    static const FGameplayTag ExperienceTag = FGameplayTag::RequestGameplayTag(FName("Data.Experience"));
    EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(ExperienceTag, BaseExperience);

    KillerAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_OwnerOnly, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_OwnerOnly, REPNOTIFY_OnChanged);
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}
