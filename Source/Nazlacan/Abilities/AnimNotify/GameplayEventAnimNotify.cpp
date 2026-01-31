#include "GameplayEventAnimNotify.h"
#include "AbilitySystemInterface.h"
#include "GameplayEventAnimNotifyState.h"
#include "Nazlacan/Macros.h"

void UGameplayEventAnimNotify::Notify(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference
) {
    Super::Notify(MeshComp, Animation, EventReference);

    AActor* Owner = MeshComp->GetOwner();
    const IAbilitySystemInterface* ActorWithComponent = Cast<IAbilitySystemInterface>(Owner);
    if (!ensure(ActorWithComponent)) return;
    UAbilitySystemComponent* AbilitySystemComponent = ActorWithComponent->GetAbilitySystemComponent();
    returnIfNull(AbilitySystemComponent);

    FGameplayEventData Data;
    Data.EventTag = Event;
    Data.Instigator = AbilitySystemComponent->GetOwnerActor();
    Data.Target = AbilitySystemComponent->GetOwnerActor();
    AbilitySystemComponent->HandleGameplayEvent(Event, &Data);
}
