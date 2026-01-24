#include "BaseGameplayAbility.h"
#include "Nazlacan/Characters/MainCharacter.h"

FVector UBaseGameplayAbility::GetIntendedDirection() const {
    AActor* Character = GetAvatarActorFromActorInfo();
    FVector Direction = Character->GetActorForwardVector();

    if (const AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(Character); PlayerCharacter != nullptr) {
        Direction = PlayerCharacter->GetIntendedDirection();

        if (Direction.IsNearlyZero()) {
            Direction = Character->GetActorForwardVector();
        }
        Direction = Direction.GetSafeNormal();
    }

    return Direction;
}

void UBaseGameplayAbility::LookAtDirection(const FVector WorldDirection) const {
    AActor* Actor = GetAvatarActorFromActorInfo();
    FRotator Rotation = WorldDirection.Rotation();
    Rotation.Pitch = 0;
    Rotation.Roll = 0;
    Actor->SetActorRotation(Rotation);
}
