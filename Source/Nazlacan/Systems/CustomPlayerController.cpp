// ReSharper disable CppMemberFunctionMayBeConst
#include "CustomPlayerController.h"
#include "CustomPlayerState.h"
#include "EnhancedInputComponent.h"
#include "Nazlacan/Macros.h"

void ACustomPlayerController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
    SetControlledCharacter(InPawn);

    ACustomPlayerState* State = GetPlayerState<ACustomPlayerState>();
    returnIfNull(State);
    State->EquipDefaultWeapons();
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
    if (InPawn == nullptr) {
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

    Input->BindAction(JumpInput, ETriggerEvent::Started, this, &ACustomPlayerController::OnJumpPressed);
    Input->BindAction(JumpInput, ETriggerEvent::Completed, this, &ACustomPlayerController::OnJumpReleased);

    Input->BindAction(SprintInput, ETriggerEvent::Started, this, &ACustomPlayerController::OnSprintPressed);
    Input->BindAction(SprintInput, ETriggerEvent::Completed, this, &ACustomPlayerController::OnSprintReleased);
}

void ACustomPlayerController::OnLookInput(const FInputActionValue& Value) {
    const FVector2D Vector = Value.Get<FVector2D>();
    AddYawInput(Vector.X);
    AddPitchInput(Vector.Y);
}

void ACustomPlayerController::OnMoveInput(const FInputActionValue& Value) {
    const FVector2D MovementDirection = Value.Get<FVector2D>();

    AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    if (PlayerCharacter == nullptr) return;

    const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    PlayerCharacter->AddMovementInput(ForwardDirection, MovementDirection.Y);
    PlayerCharacter->AddMovementInput(RightDirection, MovementDirection.X);
}

void ACustomPlayerController::OnJumpPressed() {
    if (AMainCharacter* PlayerCharacter = ControlledCharacter.Get(); PlayerCharacter != nullptr) {
        PlayerCharacter->Jump();
    }
}

void ACustomPlayerController::OnJumpReleased() {
    if (AMainCharacter* PlayerCharacter = ControlledCharacter.Get(); PlayerCharacter != nullptr) {
        PlayerCharacter->StopJumping();
    }
}

void ACustomPlayerController::OnSprintPressed() {
    if (AMainCharacter* PlayerCharacter = ControlledCharacter.Get(); PlayerCharacter != nullptr) {
        PlayerCharacter->StartSprinting();
    }
}

void ACustomPlayerController::OnSprintReleased() {
    if (AMainCharacter* PlayerCharacter = ControlledCharacter.Get(); PlayerCharacter != nullptr) {
        PlayerCharacter->StopSprinting();
    }
}
