#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Nazlacan/Macros.h"

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;
    SetupCamera();
}

void AMainCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	LoadSockets();
}

void AMainCharacter::SetupCamera() {
	USpringArmComponent* CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	UCameraComponent* FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMainCharacter::LoadSockets() {
	const USkeletalMeshComponent* CharacterMesh = GetMesh();
	returnIfNull(CharacterMesh);

	const TStaticArray<FName, 2> SocketNames = { RightHandSocketName, LeftHandSocketName };
	for (const FName SocketName : SocketNames) {
		if (!CharacterMesh->DoesSocketExist(SocketName)) {
			UE_LOG(LogTemp, Error, TEXT("Failed to find Socket %s. It does not exist on mesh %s"), *SocketName.ToString(), *CharacterMesh->GetName());
			return;
		}
	}
}

void AMainCharacter::StopJumping() {
	Super::StopJumping();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent && MovementComponent->Velocity.Z > 0) {
		MovementComponent->Velocity.Z *= JumpBreakMultiplier;
	}
}

void AMainCharacter::StartSprinting() const {
	if (GetCharacterMovement()->IsFalling()) return;
	GetCharacterMovement()->MaxWalkSpeed = MaxSprintingSpeed;
}

void AMainCharacter::StopSprinting() const {
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}
