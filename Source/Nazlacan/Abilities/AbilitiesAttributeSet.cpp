#include "AbilitiesAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Macros.h"
#include "Net/UnrealNetwork.h"

void UAbilitiesAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetSprintSpeedAttribute()) {
        const ACharacter* AvatarCharacter = Cast<ACharacter>(Data.Target.GetAvatarActor());
        returnIfNull(AvatarCharacter);
        AvatarCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed.GetCurrentValue();
    }
}

void UAbilitiesAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION_NOTIFY(UAbilitiesAttributeSet, SprintSpeed, COND_None, REPNOTIFY_OnChanged);
}

void UAbilitiesAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) {
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAbilitiesAttributeSet, SprintSpeed, OldMovementSpeed);
}
