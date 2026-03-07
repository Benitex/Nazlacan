#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "Nazlacan/Systems/CustomPlayerState.h"
#include "Nazlacan/Equipment/Weapons/CombatStyle.h"
#include "MainCharacterAnimInstance.generated.h"

UCLASS()
class NAZLACAN_API UMainCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()

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

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AMainCharacter> Character;

	UPROPERTY(Transient)
	TWeakObjectPtr<ACustomPlayerState> AnimatedPlayerState;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void LoadCharacter();
	void SetPitchAndYaw();
};
