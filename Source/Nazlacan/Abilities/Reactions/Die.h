#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Die.generated.h"

UCLASS()
class NAZLACAN_API UDie : public UGameplayAbility {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetDeathMontage() const;
};
