#include "MainCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Macros.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation() {
    Super::NativeInitializeAnimation();
    LoadCharacter();
}

void UMainCharacterAnimInstance::LoadCharacter() {
    AnimatedCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
    if (AnimatedCharacter) {
        AnimatedCharacterMovementComponent = Cast<UCharacterMovementComponent>(AnimatedCharacter->GetMovementComponent());

        USkeletalMeshComponent* AnimatedCharacterMesh = AnimatedCharacter->GetMesh();
        returnIfNull(AnimatedCharacterMesh);
        AnimatedCharacterMesh->HideBoneByName(TEXT("bow_base"), EPhysBodyOp::PBO_None);
        AnimatedCharacterMesh->HideBoneByName(TEXT("arrow_nock"), EPhysBodyOp::PBO_None);
    }
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds) {
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!AnimatedCharacter || !AnimatedCharacterMovementComponent) LoadCharacter();
    if (!AnimatedCharacter || !AnimatedCharacterMovementComponent) return;

    Speed = AnimatedCharacter->GetVelocity().Size();
    bIsInAir = AnimatedCharacterMovementComponent->IsFalling();
    SetPitchAndYaw();
}

void UMainCharacterAnimInstance::SetPitchAndYaw() {
    const FRotator CharacterRotator = AnimatedCharacter->GetActorRotation();
    const FRotator CameraRotator = AnimatedCharacter->GetBaseAimRotation();
    FRotator FinalRotator = CameraRotator - CharacterRotator;
    FinalRotator.Normalize();

    Pitch = FinalRotator.Pitch;
    Yaw = FinalRotator.Yaw;
}
