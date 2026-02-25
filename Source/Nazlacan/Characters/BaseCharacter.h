#pragma once

#include "CoreMinimal.h"
#include "Nazlacan/Systems/Corruption/Sun.h"
#include "UObject/Interface.h"
#include "BaseCharacter.generated.h"

UINTERFACE(MinimalAPI)
class UBaseCharacter : public UInterface {
	GENERATED_BODY()
};

class NAZLACAN_API IBaseCharacter {
	GENERATED_BODY()

public:
	virtual float GetDefaultCriticalChance() const = 0;

    virtual ESun GetDominantSun() const = 0;
	virtual float GetCorruptionPercent() const = 0;

	virtual float GetExperienceDroppedOnDeath() const = 0;

	virtual UAnimMontage* GetHitReactMontage() const = 0;
	virtual UAnimMontage* GetDeathMontage() const = 0;
};
