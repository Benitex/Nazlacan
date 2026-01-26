#include "BaseGameplayAbility.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/MainCharacter.h"

UBaseGameplayAbility::UBaseGameplayAbility() {
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBaseGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    Super::OnAvatarSet(ActorInfo, Spec);

    AActor* Actor = ActorInfo->AvatarActor.Get();
    returnIfNull(Actor);
    MainCharacter = Cast<AMainCharacter>(Actor);
}

FVector UBaseGameplayAbility::GetIntendedDirection() const {
    if (!MainCharacter.IsValid()) {
        return GetAvatarActorFromActorInfo()->GetActorForwardVector();
    }

    FVector Direction = MainCharacter->GetMovementIntendedDirection();
    if (Direction.IsNearlyZero()) {
        Direction = MainCharacter->GetActorForwardVector();
    }

    return Direction.GetSafeNormal();
}

void UBaseGameplayAbility::LookAtDirection(const FVector WorldDirection) const {
    FRotator Rotation = WorldDirection.Rotation();
    Rotation.Pitch = 0;
    Rotation.Roll = 0;
    AActor* Actor = GetAvatarActorFromActorInfo();
    Actor->SetActorRotation(Rotation);
}
