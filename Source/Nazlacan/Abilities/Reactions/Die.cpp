#include "Die.h"
#include "Nazlacan/Characters/BaseCharacter.h"

UAnimMontage* UDie::GetDeathMontage() const {
    AActor* Actor = GetAvatarActorFromActorInfo();
    const IBaseCharacter* Character = Cast<IBaseCharacter>(Actor);
    if (!ensure(Character)) return nullptr;
    return Character->GetDeathMontage();
}
