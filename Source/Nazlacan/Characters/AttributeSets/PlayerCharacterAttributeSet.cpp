#include "PlayerCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

void UPlayerCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetEnergyAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0, GetMaxEnergy());
    } else if (Attribute == GetSkullCountAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0, GetMaxSkullCount());
    }
}

void UPlayerCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetEnergyAttribute()) {
        SetEnergy(FMath::Clamp(GetEnergy(), 0, GetMaxEnergy()));

        if (GetEnergy() <= 0) {
            static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Event.Energy.Empty"));
            FGameplayEventData EventData;
            EventData.EventTag = Tag;
            EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
            EventData.Target = Data.Target.GetAvatarActor();

            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), Tag, EventData);
        }

    } else if (Data.EvaluatedData.Attribute == GetMaxEnergyAttribute()) {
        if (Data.EvaluatedData.Magnitude > 0) {
            SetEnergy(GetMaxEnergy() + Data.EvaluatedData.Magnitude);
        } else {
            SetEnergy(FMath::Clamp(GetEnergy(), 0, GetMaxEnergy()));
        }

    } else if (Data.EvaluatedData.Attribute == GetSkullCountAttribute()) {
        SetSkullCount(FMath::Clamp(GetSkullCount(), 0, GetMaxSkullCount()));
    } else if (Data.EvaluatedData.Attribute == GetMaxSkullCountAttribute()) {
        if (Data.EvaluatedData.Magnitude > 0) {
            SetSkullCount(GetMaxSkullCount() + Data.EvaluatedData.Magnitude);
        } else {
            SetSkullCount(GetMaxSkullCount() + Data.EvaluatedData.Magnitude);
        }
    }
}

void UPlayerCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerCharacterAttributeSet, MaxEnergy, COND_OwnerOnly, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerCharacterAttributeSet, Energy, COND_OwnerOnly, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerCharacterAttributeSet, Level, COND_OwnerOnly, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerCharacterAttributeSet, Experience, COND_OwnerOnly, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerCharacterAttributeSet, MaxSkullCount, COND_OwnerOnly, REPNOTIFY_OnChanged);
    DOREPLIFETIME_CONDITION_NOTIFY(UPlayerCharacterAttributeSet, SkullCount, COND_OwnerOnly, REPNOTIFY_OnChanged);
}

void UPlayerCharacterAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UPlayerCharacterAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, Energy, OldEnergy);
}

void UPlayerCharacterAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, Level, OldLevel);
}

void UPlayerCharacterAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, Experience, OldExperience);
}

void UPlayerCharacterAttributeSet::OnRep_MaxSkullCount(const FGameplayAttributeData& OldMaxSkullCount) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, MaxSkullCount, OldMaxSkullCount);
}

void UPlayerCharacterAttributeSet::OnRep_SkullCount(const FGameplayAttributeData& OldSkullCount) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerCharacterAttributeSet, SkullCount, OldSkullCount);
}
