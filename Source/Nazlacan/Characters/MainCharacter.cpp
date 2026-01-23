#include "MainCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Systems/CustomPlayerState.h"

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

void AMainCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	LoadSockets();
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

void AMainCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	ACustomPlayerState* State = GetPlayerState<ACustomPlayerState>();
	LoadAbilitySystemComponent(State);
	State->SetDefaultAbilitiesAndEffects();
}

void AMainCharacter::OnRep_PlayerState() {
	Super::OnRep_PlayerState();
	ACustomPlayerState* State = GetPlayerState<ACustomPlayerState>();
	LoadAbilitySystemComponent(State);
}

void AMainCharacter::LoadAbilitySystemComponent(ACustomPlayerState* FromState) {
	AbilitySystemComponent = FromState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(FromState, this);
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
	static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Active.Sprint"));
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(Tag));
}

void AMainCharacter::StopSprinting() const {
	static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Active.Sprint"));
	static const FGameplayTagContainer Container(Tag);
	AbilitySystemComponent->CancelAbilities(&Container);
}

void AMainCharacter::StartDodging() const {
	if (GetCharacterMovement()->IsFalling()) return;
	static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Active.Roll"));
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(Tag));
}
