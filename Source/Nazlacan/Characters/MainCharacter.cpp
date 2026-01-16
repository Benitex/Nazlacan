#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;
    SetupCamera();
}

void AMainCharacter::SetupCamera() {
	USpringArmComponent* CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	UCameraComponent* FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMainCharacter::StopJumping() {
	Super::StopJumping();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent && MovementComponent->Velocity.Z > 0) {
		MovementComponent->Velocity.Z *= JumpBreakMultiplier;
	}
}

FName AMainCharacter::GetRightHandSocketName() const { return RightHandSocketName; }
FName AMainCharacter::GetLeftHandSocketName() const { return LeftHandSocketName; }
