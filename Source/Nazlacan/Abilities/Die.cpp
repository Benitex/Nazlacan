#include "Die.h"
#include "Nazlacan/Characters/MainCharacter.h"

UAnimMontage* UDie::GetDeathMontage() const {
    AActor* Actor = GetAvatarActorFromActorInfo();

    if (const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Actor)) {
        return MainCharacter->GetDeathMontage();
    }

    return nullptr;
}
