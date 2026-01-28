// ReSharper disable CppMemberFunctionMayBeConst
#include "CustomPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Nazlacan/Macros.h"

void ACustomPlayerController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
    SetControlledCharacter(InPawn);
}

void ACustomPlayerController::OnUnPossess() {
    Super::OnUnPossess();
    SetControlledCharacter(nullptr);
}

// Updates [ControlledCharacter] on client
void ACustomPlayerController::OnRep_Pawn() {
    Super::OnRep_Pawn();
    SetControlledCharacter(GetPawn());
}

void ACustomPlayerController::SetControlledCharacter(APawn* InPawn) {
    if (!InPawn) {
        ControlledCharacter = nullptr;
        return;
    }

    ControlledCharacter = CastChecked<AMainCharacter>(InPawn);

    if (IsLocalPlayerController()) {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
        returnIfNull(Subsystem);
        Subsystem->AddMappingContext(InputMap, 0);
    }
}

void ACustomPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(Super::InputComponent);
    returnIfNull(Input);

    Input->BindAction(LookInput, ETriggerEvent::Triggered, this, &ACustomPlayerController::OnLookInput);
    Input->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ACustomPlayerController::OnMoveInput);
    Input->BindAction(MoveInput, ETriggerEvent::Completed, this, &ACustomPlayerController::OnMoveInputReleased);

    Input->BindAction(JumpInput, ETriggerEvent::Started, this, &ACustomPlayerController::OnJumpPressed);
    Input->BindAction(JumpInput, ETriggerEvent::Completed, this, &ACustomPlayerController::OnJumpReleased);

    Input->BindAction(SprintInput, ETriggerEvent::Started, this, &ACustomPlayerController::OnSprintPressed);
    Input->BindAction(SprintInput, ETriggerEvent::Completed, this, &ACustomPlayerController::OnSprintReleased);

    Input->BindAction(Attack1Input, ETriggerEvent::Started, this, &ACustomPlayerController::OnAttackButtonPressed, 1);
    Input->BindAction(Attack2Input, ETriggerEvent::Started, this, &ACustomPlayerController::OnAttackButtonPressed, 2);
    Input->BindAction(Attack3Input, ETriggerEvent::Started, this, &ACustomPlayerController::OnAttackButtonPressed, 3);
    Input->BindAction(DodgeInput, ETriggerEvent::Started, this, &ACustomPlayerController::OnDodgePressed);
}

void ACustomPlayerController::OnLookInput(const FInputActionValue& Value) {
    const FVector2D Vector = Value.Get<FVector2D>();
    AddYawInput(Vector.X);
    AddPitchInput(Vector.Y);
}

void ACustomPlayerController::OnMoveInput(const FInputActionValue& Value) {
    const FVector2D MovementDirection = Value.Get<FVector2D>();

    AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    if (!PlayerCharacter) return;

    const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    const FVector IntendedDirection = ForwardDirection * MovementDirection.Y + RightDirection * MovementDirection.X;
    PlayerCharacter->SetMovementIntendedDirection(IntendedDirection.GetSafeNormal());
    if (!PlayerCharacter->CanMove()) return;

    PlayerCharacter->AddMovementInput(ForwardDirection, MovementDirection.Y);
    PlayerCharacter->AddMovementInput(RightDirection, MovementDirection.X);
}

void ACustomPlayerController::OnMoveInputReleased() {
    if (AMainCharacter* PlayerCharacter = ControlledCharacter.Get()) {
        PlayerCharacter->SetMovementIntendedDirection(FVector::ZeroVector);
    }
}

void ACustomPlayerController::OnAttackButtonPressed(const FInputActionValue& Value, const int ButtonNumber) {
    AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    if (!PlayerCharacter) return;
    if (!CombatStyleCombos.Contains(PlayerCharacter->GetState()->GetCombatStyle())) return;

    FGameplayTag AttackTag;
    const FCombatStyleCombo CombatStyleCombo = CombatStyleCombos[PlayerCharacter->GetState()->GetCombatStyle()];

    if (PlayerCharacter->IsFalling()) {
        AttackTag = CombatStyleCombo.AerialAttack;
    } else {
        TMap<FGameplayTag, FGameplayTag> InputCombos;
        if (ButtonNumber == 1) {
            InputCombos = CombatStyleCombo.Attack1Input;
        } else if (ButtonNumber == 2) {
            InputCombos = CombatStyleCombo.Attack2Input;
        } else if (ButtonNumber == 3) {
            InputCombos = CombatStyleCombo.Attack3Input;
        } else {
            UE_LOG(LogTemp, Warning, TEXT("Invalid attack button number (%d) assigned in Controller"), ButtonNumber);
            return;
        }

        AttackTag = InputCombos.FindRef(PlayerCharacter->GetLastAttack(), FGameplayTag::EmptyTag);
    }

    if (AttackTag == FGameplayTag::EmptyTag) return;
    PlayerCharacter->PrepareAttackWithTag(AttackTag);
}

void ACustomPlayerController::OnJumpPressed() {
    AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    if (!PlayerCharacter || !PlayerCharacter->CanMove() || PlayerCharacter->IsFalling()) return;

    PlayerCharacter->Jump();
}

void ACustomPlayerController::OnJumpReleased() {
    if (AMainCharacter* PlayerCharacter = ControlledCharacter.Get()) {
        PlayerCharacter->StopJumping();
    }
}

void ACustomPlayerController::OnSprintPressed() {
    const AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    if (!PlayerCharacter || PlayerCharacter->IsFalling()) return;
    if (PlayerCharacter->GetMovementIntendedDirection().IsNearlyZero()) return;

    PlayerCharacter->StartSprinting();
}

void ACustomPlayerController::OnSprintReleased() {
    if (const AMainCharacter* PlayerCharacter = ControlledCharacter.Get()) {
        PlayerCharacter->StopSprinting();
    }
}

void ACustomPlayerController::OnDodgePressed() {
    const AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    if (!PlayerCharacter || PlayerCharacter->IsFalling()) return;

	static const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Active.Roll"));
    PlayerCharacter->ActivateAbilityWithTag(Tag);
}
