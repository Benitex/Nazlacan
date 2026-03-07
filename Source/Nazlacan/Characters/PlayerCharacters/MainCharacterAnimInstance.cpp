#include "MainCharacterAnimInstance.h"
#include "Nazlacan/Macros.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Systems/CustomPlayerState.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation() {
    Super::NativeInitializeAnimation();
    LoadCharacter();
}

void UMainCharacterAnimInstance::LoadCharacter() {
    Character = Cast<AMainCharacter>(TryGetPawnOwner());
    if (!ensure(Character.IsValid())) return;

    AnimatedPlayerState = Character->GetPlayerState<ACustomPlayerState>();

    USkeletalMeshComponent* AnimatedCharacterMesh = Character->GetMesh();
    returnIfNull(AnimatedCharacterMesh);
    AnimatedCharacterMesh->HideBoneByName(TEXT("bow_base"), EPhysBodyOp::PBO_None);
    AnimatedCharacterMesh->HideBoneByName(TEXT("arrow_nock"), EPhysBodyOp::PBO_None);
    AnimatedCharacterMesh->HideBoneByName(TEXT("feathers"), EPhysBodyOp::PBO_None);
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds) {
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Character.IsValid() || !AnimatedPlayerState.IsValid()) {
        LoadCharacter();
    }
    if (!Character.IsValid() || !AnimatedPlayerState.IsValid()) return;

    Speed = Character->GetVelocity().Size();
    bIsInAir = Character->GetCharacterMovement()->IsFalling();
    SetPitchAndYaw();
    CombatStyle = AnimatedPlayerState->GetCombatStyle();
}

void UMainCharacterAnimInstance::SetPitchAndYaw() {
    const FRotator CharacterRotator = Character->GetActorRotation();
    const FRotator CameraRotator = Character->GetBaseAimRotation();
    FRotator FinalRotator = CameraRotator - CharacterRotator;
    FinalRotator.Normalize();

    Pitch = FinalRotator.Pitch;
    Yaw = FinalRotator.Yaw;
}
