#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "Nazlacan/Systems/CustomPlayerState.h"
#include "Nazlacan/Weapons/CombatStyle.h"
#include "MainCharacterAnimInstance.generated.h"

UCLASS()
class NAZLACAN_API UMainCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()

	UPROPERTY(Transient)
	TObjectPtr<AMainCharacter> AnimatedCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> AnimatedCharacterMovementComponent;

	UPROPERTY(Transient)
	TObjectPtr<ACustomPlayerState> AnimatedPlayerState;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	double Yaw;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	double Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	ECombatStyle CombatStyle;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void LoadCharacter();
	void SetPitchAndYaw();
};
