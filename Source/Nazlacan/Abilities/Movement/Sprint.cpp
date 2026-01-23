#include "Sprint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/MainCharacter.h"

void USprint::ResetMovementSpeed() const {
    const AMainCharacter* Character = Cast<AMainCharacter>(GetAvatarActorFromActorInfo());
    returnIfNull(Character);
    Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetDefaultMaxWalkSpeed();
}
