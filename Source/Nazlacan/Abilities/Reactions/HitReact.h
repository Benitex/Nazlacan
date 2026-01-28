#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HitReact.generated.h"

UCLASS()
class NAZLACAN_API UHitReact : public UGameplayAbility {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetHitMontage() const;
};
