#include "BaseGameplayAbility.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/PlayerCharacters/MainCharacter.h"

UBaseGameplayAbility::UBaseGameplayAbility() {
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBaseGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    Super::OnAvatarSet(ActorInfo, Spec);

    AActor* Actor = ActorInfo->AvatarActor.Get();
    returnIfNull(Actor);
    Character = Cast<IBaseCharacter>(Actor);
}

FVector UBaseGameplayAbility::GetIntendedDirection() const {
    if (const AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character.Get()); IsValid(MainCharacter)) {
        FVector Direction = MainCharacter->GetMovementIntendedDirection();
        if (Direction.IsNearlyZero()) {
            Direction = MainCharacter->GetActorForwardVector();
        }

        return Direction.GetSafeNormal();
    }

    if (const APawn* Pawn = Cast<APawn>(Character.Get())) {
        FVector Direction = Pawn->GetLastMovementInputVector();
        if (Direction.IsNearlyZero()) {
            Direction = GetAvatarActorFromActorInfo()->GetActorForwardVector();
        }

        return Direction.GetSafeNormal();
    }

    return GetAvatarActorFromActorInfo()->GetActorForwardVector();
}

void UBaseGameplayAbility::LookAtDirection(const FVector WorldDirection) const {
    FRotator Rotation = WorldDirection.Rotation();
    Rotation.Pitch = 0;
    Rotation.Roll = 0;
    AActor* Actor = GetAvatarActorFromActorInfo();
    Actor->SetActorRotation(Rotation);
}

float UBaseGameplayAbility::GetActorSpeed() const {
    const AActor* Actor = GetAvatarActorFromActorInfo();
    return Actor->GetVelocity().Size();
}
