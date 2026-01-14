#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

UCLASS()
class NAZLACAN_API UMainCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()

	UPROPERTY(Transient)
	AMainCharacter* AnimatedCharacter;

	UPROPERTY(Transient)
	UCharacterMovementComponent* AnimatedCharacterMovementComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	double Yaw;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	double Pitch;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void LoadCharacter();
	void SetPitchAndYaw();
};
