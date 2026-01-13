#include "MainCharacter.h"

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;
    SetupCamera();
}

void AMainCharacter::SetupCamera() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = bShouldLagCameraOnMovement;
	CameraBoom->bEnableCameraRotationLag = bShouldLagCameraOnMovement;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMainCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMainCharacter::Move(float Forward, float Right) {
	
}
