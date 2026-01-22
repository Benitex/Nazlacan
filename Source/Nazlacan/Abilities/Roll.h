#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Roll.generated.h"

UCLASS()
class NAZLACAN_API URoll : public UGameplayAbility {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	FVector GetDirection() const;

	UFUNCTION(BlueprintCallable)
	void LookAtRollDirection() const;

	UFUNCTION(BlueprintCallable)
	void LockRotation(bool bShouldLock = true) const;
};
