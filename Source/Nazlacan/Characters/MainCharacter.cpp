#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Nazlacan/Macros.h"

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

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AMainCharacter::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMainCharacter::StopJumping() {
	Super::StopJumping();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent && MovementComponent->Velocity.Z > 0) {
		MovementComponent->Velocity.Z *= JumpBreakMultiplier;
	}
}
