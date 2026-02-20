#include "MainCharacterAnimInstance.h"
#include "Nazlacan/Macros.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Systems/CustomPlayerState.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation() {
    Super::NativeInitializeAnimation();
    LoadCharacter();
}

void UMainCharacterAnimInstance::LoadCharacter() {
    AnimatedCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
    if (IsValid(AnimatedCharacter)) {
        AnimatedCharacterMovementComponent = Cast<UCharacterMovementComponent>(AnimatedCharacter->GetMovementComponent());
        AnimatedPlayerState = AnimatedCharacter->GetPlayerState<ACustomPlayerState>();

        USkeletalMeshComponent* AnimatedCharacterMesh = AnimatedCharacter->GetMesh();
        returnIfNull(AnimatedCharacterMesh);
        AnimatedCharacterMesh->HideBoneByName(TEXT("bow_base"), EPhysBodyOp::PBO_None);
        AnimatedCharacterMesh->HideBoneByName(TEXT("arrow_nock"), EPhysBodyOp::PBO_None);
        AnimatedCharacterMesh->HideBoneByName(TEXT("feathers"), EPhysBodyOp::PBO_None);
    }
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds) {
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!IsValid(AnimatedCharacter) || !IsValid(AnimatedCharacterMovementComponent) || !IsValid(AnimatedPlayerState)) {
        LoadCharacter();
    }
    if (!IsValid(AnimatedCharacter) || !IsValid(AnimatedCharacterMovementComponent) || !IsValid(AnimatedPlayerState)) return;

    Speed = AnimatedCharacter->GetVelocity().Size();
    bIsInAir = AnimatedCharacterMovementComponent->IsFalling();
    SetPitchAndYaw();
    CombatStyle = AnimatedPlayerState->GetCombatStyle();
}

void UMainCharacterAnimInstance::SetPitchAndYaw() {
    const FRotator CharacterRotator = AnimatedCharacter->GetActorRotation();
    const FRotator CameraRotator = AnimatedCharacter->GetBaseAimRotation();
    FRotator FinalRotator = CameraRotator - CharacterRotator;
    FinalRotator.Normalize();

    Pitch = FinalRotator.Pitch;
    Yaw = FinalRotator.Yaw;
}
