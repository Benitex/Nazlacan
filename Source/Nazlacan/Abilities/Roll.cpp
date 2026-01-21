#include "Roll.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Characters/MainCharacter.h"

FVector URoll::GetDirection() const {
    AActor* Character = GetAvatarActorFromActorInfo();
    FVector Direction = Character->GetActorForwardVector();

    if (const AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(Character); PlayerCharacter != nullptr) {
        Direction = PlayerCharacter->GetLastMovementInputVector();

        if (Direction.IsNearlyZero()) {
            Direction = Character->GetActorForwardVector();
        } else {
            Direction.Normalize();
        }
    }

    return Direction;
}

void URoll::LockRotation(const bool bShouldLock) const {
    const AActor* Character = GetAvatarActorFromActorInfo();
    const AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(Character);
    if (PlayerCharacter == nullptr) return;
    PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !bShouldLock;
}
