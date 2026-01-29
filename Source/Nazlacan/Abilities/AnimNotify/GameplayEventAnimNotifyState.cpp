#include "GameplayEventAnimNotifyState.h"
#include "AbilitySystemInterface.h"
#include "Nazlacan/Macros.h"

void UGameplayEventAnimNotifyState::NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const float TotalDuration,
    const FAnimNotifyEventReference& EventReference
) {
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystem(MeshComp);
    returnIfNull(AbilitySystemComponent);
    ActivateEvent(AbilitySystemComponent, StartEvent);
}

void UGameplayEventAnimNotifyState::NotifyEnd(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference
) {
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystem(MeshComp);
    returnIfNull(AbilitySystemComponent);
    ActivateEvent(AbilitySystemComponent, EndEvent);
}

UAbilitySystemComponent* UGameplayEventAnimNotifyState::GetAbilitySystem(const USkeletalMeshComponent* FromMesh) {
    AActor* Owner = FromMesh->GetOwner();
    const IAbilitySystemInterface* ActorWithComponent = Cast<IAbilitySystemInterface>(Owner);
    if (!ensure(ActorWithComponent)) return nullptr;
    return ActorWithComponent->GetAbilitySystemComponent();
}

void UGameplayEventAnimNotifyState::ActivateEvent(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag Tag) {
    FGameplayEventData Data;
    Data.EventTag = Tag;
    Data.Instigator = AbilitySystemComponent->GetOwnerActor();
    Data.Target = AbilitySystemComponent->GetOwnerActor();
    AbilitySystemComponent->HandleGameplayEvent(Tag, &Data);
}
