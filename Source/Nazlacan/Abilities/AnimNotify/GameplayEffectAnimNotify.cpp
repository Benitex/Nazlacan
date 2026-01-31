#include "GameplayEffectAnimNotify.h"
#include "Nazlacan/Macros.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

void UGameplayEffectAnimNotify::Notify(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference
) {
    Super::Notify(MeshComp, Animation, EventReference);

    const IAbilitySystemInterface* ActorWithComponent = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
    if (!ensure(ActorWithComponent)) return;
    UAbilitySystemComponent* AbilitySystemComponent = ActorWithComponent->GetAbilitySystemComponent();
    returnIfNull(AbilitySystemComponent);

    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(MeshComp->GetOwner());
    const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, Context);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}
