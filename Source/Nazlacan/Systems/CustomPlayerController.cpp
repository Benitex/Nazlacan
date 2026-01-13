#include "CustomPlayerController.h"
#include "EnhancedInputComponent.h"

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
    if (InPawn == nullptr) {
        ControlledCharacter = nullptr;
        return;
    }

    ControlledCharacter = CastChecked<AMainCharacter>(InPawn);

    if (IsLocalPlayerController()) {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
        ensure(Subsystem != nullptr);
        Subsystem->AddMappingContext(InputMap, 0);
    }
}

void ACustomPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(Super::InputComponent);
    ensure(Input != nullptr);

    Input->BindAction(LookInput, ETriggerEvent::Triggered, this, &ACustomPlayerController::OnLookInput);
    Input->BindAction(MoveInput, ETriggerEvent::Triggered, this, &ACustomPlayerController::OnMoveInput);
}

void ACustomPlayerController::OnLookInput(const FInputActionValue& Value) {
    const FVector2D Vector = Value.Get<FVector2D>();
    AddYawInput(Vector.X);
    AddPitchInput(Vector.Y);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACustomPlayerController::OnMoveInput(const FInputActionValue& Value) {
    const FVector2D MovementDirection = Value.Get<FVector2D>();

    AMainCharacter* PlayerCharacter = ControlledCharacter.Get();
    ensure(PlayerCharacter != nullptr);

    const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    PlayerCharacter->AddMovementInput(ForwardDirection, MovementDirection.Y);
    PlayerCharacter->AddMovementInput(RightDirection, MovementDirection.X);
}
