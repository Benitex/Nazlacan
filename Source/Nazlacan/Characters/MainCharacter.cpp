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

	if (RightHandWeapon.DataTable != nullptr && RightHandWeapon.RowName != NAME_None) {
		EquipWeapon(RightHandWeapon, true);
	}
	if (LeftHandWeapon.DataTable != nullptr && LeftHandWeapon.RowName != NAME_None) {
		EquipWeapon(LeftHandWeapon, false);
	}
}

void AMainCharacter::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMainCharacter::EquipWeapon(FDataTableRowHandle& WeaponRowHandle, const bool bIsRightHand) {
	// TODO remove previous weapon if any

	static const FString ContextString(TEXT("Equipping Weapon"));
	const FWeaponData* WeaponData = WeaponRowHandle.DataTable->FindRow<FWeaponData>(WeaponRowHandle.RowName, ContextString);
	returnIfNull(WeaponData);

	const FName SocketName = bIsRightHand ? RightHandSocketName : LeftHandSocketName;
	const FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName);
	AWeapon* SpawnedWeapon = AWeapon::Spawn(*WeaponData, SocketTransform, this);
	returnIfNull(SpawnedWeapon);

	SpawnedWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		SocketName
	);
}

void AMainCharacter::StopJumping() {
	Super::StopJumping();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent && MovementComponent->Velocity.Z > 0) {
		MovementComponent->Velocity.Z *= JumpBreakMultiplier;
	}
}
