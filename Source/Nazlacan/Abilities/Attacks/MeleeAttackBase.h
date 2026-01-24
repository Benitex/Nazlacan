#pragma once

#include "CoreMinimal.h"
#include "Nazlacan/Abilities/BaseGameplayAbility.h"
#include "MeleeAttackBase.generated.h"

UCLASS()
class NAZLACAN_API UMeleeAttackBase : public UBaseGameplayAbility {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float MotionValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0.0"))
	float MovementTowardsTargetIntensity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AnimationBaseSpeed = 1;
};
