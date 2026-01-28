#include "Die.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/MainCharacter.h"

UAnimMontage* UDie::GetDeathMontage() const {
    AActor* Actor = GetAvatarActorFromActorInfo();
    const IBaseCharacter* Character = Cast<IBaseCharacter>(Actor);
    returnIfNull(Character) nullptr;
    return Character->GetDeathMontage();
}
