#include "MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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

void AMainCharacter::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	USkeletalMeshComponent* CurrentMesh = GetMesh();
	ensure(CurrentMesh != nullptr);
	CurrentMesh->HideBoneByName(TEXT("bow_base"), EPhysBodyOp::PBO_None);
	CurrentMesh->HideBoneByName(TEXT("arrow_nock"), EPhysBodyOp::PBO_None);
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
