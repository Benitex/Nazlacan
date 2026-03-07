#include "EnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation() {
    Super::NativeInitializeAnimation();
    Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(const float DeltaSeconds) {
    Super::NativeUpdateAnimation(DeltaSeconds);
    if (!Character.IsValid()) return;

    Speed = Character->GetVelocity().Size();
    bIsInAir = Character->GetMovementComponent()->IsFalling();
}
