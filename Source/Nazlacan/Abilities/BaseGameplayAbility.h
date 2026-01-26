#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Nazlacan/Characters/MainCharacter.h"
#include "BaseGameplayAbility.generated.h"

UCLASS()
class NAZLACAN_API UBaseGameplayAbility : public UGameplayAbility {
	GENERATED_BODY()

protected:
	TWeakObjectPtr<AMainCharacter> MainCharacter;

public:
	UBaseGameplayAbility();
	
protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetIntendedDirection() const;

	UFUNCTION(BlueprintCallable)
	virtual void LookAtIntendedDirection() const { LookAtDirection(GetIntendedDirection()); }

	UFUNCTION(BlueprintCallable)
	virtual void LookAtDirection(FVector WorldDirection) const;
	
	UFUNCTION(BlueprintCallable)
	virtual float GetActorSpeed() const;
};
