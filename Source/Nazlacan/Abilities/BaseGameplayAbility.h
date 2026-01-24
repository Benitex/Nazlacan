#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

UCLASS()
class NAZLACAN_API UBaseGameplayAbility : public UGameplayAbility {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	virtual FVector GetIntendedDirection() const;

	UFUNCTION(BlueprintCallable)
	virtual void LookAtIntendedDirection() const { LookAtDirection(GetIntendedDirection()); }

	UFUNCTION(BlueprintCallable)
	virtual void LookAtDirection(FVector WorldDirection) const;
};
