#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Sprint.generated.h"

UCLASS()
class NAZLACAN_API USprint : public UGameplayAbility {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void ResetMovementSpeed() const;
};
