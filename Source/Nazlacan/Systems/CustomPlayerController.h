#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Nazlacan/Characters/MainCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Nazlacan/Weapons/CombatStyleCombo.h"
#include "CustomPlayerController.generated.h"

class UInputAction;

UCLASS()
class NAZLACAN_API ACustomPlayerController : public APlayerController {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMap;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DodgeInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Attack1Input;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Attack2Input;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Attack3Input;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TMap<ECombatStyle, FCombatStyleCombo> CombatStyleCombos;

private:
	TWeakObjectPtr<AMainCharacter> ControlledCharacter;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void OnRep_Pawn() override;
	virtual void SetupInputComponent() override;

private:
	void OnLookInput(const FInputActionValue& Value);
	void OnMoveInput(const FInputActionValue& Value);
	void OnMoveInputReleased();
	void OnJumpPressed();
	void OnJumpReleased();
	void OnSprintPressed();
	void OnSprintReleased();
	void OnDodgePressed();
	void OnAttackButtonPressed(const FInputActionValue& Value, int ButtonNumber);

	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetControlledCharacter(APawn* InPawn);
};
