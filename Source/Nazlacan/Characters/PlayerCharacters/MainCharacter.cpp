#include "MainCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Nazlacan/Systems/CustomPlayerState.h"

AMainCharacter::AMainCharacter() {
    SetupCamera();
}

void AMainCharacter::SetupCamera() {
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
}

void AMainCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AMainCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	State = GetPlayerState<ACustomPlayerState>();
	GetAbilitySystemComponent()->InitAbilityActorInfo(State.Get(), this);

	State->LoadAbilitiesAndEffects();
}

void AMainCharacter::OnRep_PlayerState() {
	Super::OnRep_PlayerState();
	State = GetPlayerState<ACustomPlayerState>();
	GetAbilitySystemComponent()->InitAbilityActorInfo(State.Get(), this);
}

void AMainCharacter::ActivateEventToSelf(const FGameplayTag& Tag) const {
	FGameplayEventData Data;
	Data.EventTag = Tag;
	Data.Instigator = this;
	Data.Target = this;
	GetAbilitySystemComponent()->HandleGameplayEvent(Tag, &Data);
}

bool AMainCharacter::ActivateAbilityWithTag(const FGameplayTag& Tag) const {
	return GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(Tag));
}

void AMainCharacter::PrepareAttackWithTag(const FGameplayTag& Tag) {
	if (IsAttacking()) {
		NextAttack = Tag;
		static const FGameplayTag AttackReadyTag = FGameplayTag::RequestGameplayTag(FName("Event.Attack.NextAttackReady"));
		ActivateEventToSelf(AttackReadyTag);
	} else {
		if (ActivateAbilityWithTag(Tag)) {
			LastAttack = Tag;
		} else {
			LastAttack = FGameplayTag::EmptyTag;
			NextAttack = FGameplayTag::EmptyTag;
		}
	}
}

void AMainCharacter::TryToActivateNextAttack() {
	if (NextAttack == FGameplayTag::EmptyTag) return;
	if (ActivateAbilityWithTag(NextAttack)) {
		LastAttack = NextAttack;
		NextAttack = FGameplayTag::EmptyTag;
	} else {
		LastAttack = FGameplayTag::EmptyTag;
		NextAttack = FGameplayTag::EmptyTag;
	}
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

void AMainCharacter::AttachEquipmentToMesh(const TScriptInterface<IEquipment>& Equipment, const EEquipmentSlot Slot) {
	const FName* Socket = EquipmentSockets.Find(Slot);
	if (!Socket) {
		UE_LOG(LogTemp, Warning, TEXT("No socket found for equipment slot %d on character %s"), static_cast<uint8>(Slot), *GetName());
		return;
	}

	Equipment.GetInterface()->AttachEquipment(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, *Socket);
}
