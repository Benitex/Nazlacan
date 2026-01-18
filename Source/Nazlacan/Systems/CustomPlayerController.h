#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Nazlacan/Characters/MainCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "CustomPlayerController.generated.h"

class UInputAction;

UCLASS()
class NAZLACAN_API ACustomPlayerController : public APlayerController {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMap;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintInput;

	TWeakObjectPtr<AMainCharacter> ControlledCharacter;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void OnRep_Pawn() override;
	virtual void SetupInputComponent() override;

private:
	void OnLookInput(const FInputActionValue& Value);
	void OnMoveInput(const FInputActionValue& Value);
	void OnJumpPressed();
	void OnJumpReleased();
	void OnSprintPressed();
	void OnSprintReleased();

	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetControlledCharacter(APawn* InPawn);
};
