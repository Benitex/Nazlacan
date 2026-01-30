#include "MainCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Systems/CustomPlayerState.h"

AMainCharacter::AMainCharacter() {
	PrimaryActorTick.bCanEverTick = true;
    SetupCamera();
}

void AMainCharacter::SetupCamera() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
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
			UE_LOG(LogTemp, Error, TEXT("Failed to find Socket %s. It does not exist on %s"), *SocketName.ToString(), *GetName());
		}
	}
}

void AMainCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	State = GetPlayerState<ACustomPlayerState>();
	GetAbilitySystemComponent()->InitAbilityActorInfo(State.Get(), this);

	State->SetDefaultAbilitiesAndEffects();
}

void AMainCharacter::OnRep_PlayerState() {
	Super::OnRep_PlayerState();
	State = GetPlayerState<ACustomPlayerState>();
	GetAbilitySystemComponent()->InitAbilityActorInfo(State.Get(), this);
}

void AMainCharacter::ActivateAbilityWithTag(const FGameplayTag& Tag) const {
	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(Tag));
}

void AMainCharacter::PrepareAttackWithTag(const FGameplayTag& Tag) {
	if (IsAttacking()) {
		NextAttack = Tag;
		static const FGameplayTag AttackReadyTag = FGameplayTag::RequestGameplayTag(FName("Event.Attack.NextAttackReady"));
		GetAbilitySystemComponent()->AddLooseGameplayTag(AttackReadyTag);
	} else {
		LastAttack = Tag;
		ActivateAbilityWithTag(Tag);
	}
}

void AMainCharacter::TryToActivateNextAttack() {
	if (NextAttack == FGameplayTag::EmptyTag) return;
	LastAttack = NextAttack;
	ActivateAbilityWithTag(NextAttack);
	NextAttack = FGameplayTag::EmptyTag;
}

// ReSharper disable once CppPassValueParameterByConstReference
void AMainCharacter::RemoveLastAttack(const FGameplayTagContainer TagFilter) {
	if (TagFilter.IsEmpty() || LastAttack.MatchesAny(TagFilter)) {
		LastAttack = FGameplayTag::EmptyTag;
	}
}

void AMainCharacter::StartSprinting() const {
	static const FGameplayTagContainer Tag = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Active.Sprint")));
	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(Tag);
}

void AMainCharacter::StopSprinting() const {
	static const FGameplayTagContainer Tag = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Active.Sprint")));
	GetAbilitySystemComponent()->CancelAbilities(&Tag);
}

void AMainCharacter::StopJumping() {
	Super::StopJumping();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent && MovementComponent->Velocity.Z > 0) {
		MovementComponent->Velocity.Z *= JumpBreakMultiplier;
	}
}

FVector AMainCharacter::GetMovementIntendedDirection() const {
	if (!CanMove()) return MovementIntendedDirection;
	return GetLastMovementInputVector();
}

UAnimMontage* AMainCharacter::GetHitReactMontage() const {
	return HitReactMontages[State->GetCombatStyle()];
}

bool AMainCharacter::CanMove() const {
	static const FGameplayTag MovementLocked = FGameplayTag::RequestGameplayTag(FName("Status.MovementLocked"));
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(MovementLocked)) return false;

	static const FGameplayTag Dead = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(Dead)) return false;

	return true;
}

bool AMainCharacter::IsAttacking() const {
	static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("State.Attacking"));
	return GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag);
}
