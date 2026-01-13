#include "CustomPlayerController.h"
#include "EnhancedInputComponent.h"

void ACustomPlayerController::OnPossess(APawn* APawn) {
    Super::OnPossess(APawn);

    ControlledCharacter = CastChecked<AMainCharacter>(APawn);

    if (IsLocalPlayerController()) {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
        ensure(Subsystem != nullptr);
        Subsystem->AddMappingContext(InputMap, 0);
    }
}

void ACustomPlayerController::OnUnPossess() {
    Super::OnUnPossess();
    ControlledCharacter = nullptr;
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

void ACustomPlayerController::OnMoveInput(const FInputActionValue& Value) {
    const FVector2D MovementDirection = Value.Get<FVector2D>();


}
