#include "HitReact.h"
#include "Nazlacan/Characters/MainCharacter.h"

UAnimMontage* UHitReact::GetHitMontage() const {
    AActor* Actor = GetAvatarActorFromActorInfo();

    if (const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Actor)) {
        return MainCharacter->GetHitReactMontage();
    }

    return nullptr;
}
