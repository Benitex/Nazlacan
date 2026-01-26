#include "GameplayEffectAnimNotifyState.h"
#include "AbilitySystemInterface.h"
#include "Nazlacan/Macros.h"
#include "AbilitySystemComponent.h"

void UGameplayEffectAnimNotifyState::NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const float TotalDuration,
    const FAnimNotifyEventReference& EventReference
) {
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystem(MeshComp);
    returnIfNull(AbilitySystemComponent);

    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(MeshComp->GetOwner());
    const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, Context);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UGameplayEffectAnimNotifyState::NotifyEnd(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference
) {
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystem(MeshComp);
    returnIfNull(AbilitySystemComponent);
    AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, AbilitySystemComponent);
}

UAbilitySystemComponent* UGameplayEffectAnimNotifyState::GetAbilitySystem(const USkeletalMeshComponent* FromMesh) {
    AActor* Owner = FromMesh->GetOwner();
    const IAbilitySystemInterface* ActorWithComponent = Cast<IAbilitySystemInterface>(Owner);
    returnIfNull(ActorWithComponent) nullptr;
    return ActorWithComponent->GetAbilitySystemComponent();
}
