#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageEffectCalculation.generated.h"

UCLASS()
class NAZLACAN_API UDamageEffectCalculation : public UGameplayEffectExecutionCalculation {
	GENERATED_BODY()

public:
	UDamageEffectCalculation();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;
};
