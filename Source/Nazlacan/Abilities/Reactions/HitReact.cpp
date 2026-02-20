#include "HitReact.h"
#include "Nazlacan/Characters/BaseCharacter.h"

UAnimMontage* UHitReact::GetHitMontage() const {
    AActor* Actor = GetAvatarActorFromActorInfo();
    const IBaseCharacter* Character = Cast<IBaseCharacter>(Actor);
    if (!ensure(Character)) return nullptr;
    return Character->GetHitReactMontage();
}
