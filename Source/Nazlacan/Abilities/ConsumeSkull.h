#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ConsumeSkull.generated.h"

UCLASS()
class NAZLACAN_API UConsumeSkull : public UGameplayAbility {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> HealGameplayEffect;

protected:
	UFUNCTION(BlueprintCallable)
	void ConsumeSkull() const;

	UFUNCTION(BlueprintCallable)
	bool ShouldHeal() const;
};
