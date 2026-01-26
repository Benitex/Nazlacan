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

	// Attacks that become available by pressing Attack 1 after performing a specific attack.
	// Key: Previous Attack Tag, Value: Next Attack Tag
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FGameplayTag> Attack1Combos;

	// Attacks that become available by pressing Attack 2 after performing a specific attack.
	// Key: Previous Attack Tag, Value: Next Attack Tag
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FGameplayTag> Attack2Combos;

	// Attacks that become available by pressing Attack 3 after performing a specific attack.
	// Key: Previous Attack Tag, Value: Next Attack Tag
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FGameplayTag> Attack3Combos;

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
	void OnAttack1Pressed();

	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetControlledCharacter(APawn* InPawn);
};
