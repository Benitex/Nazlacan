#pragma once

#include "CoreMinimal.h"
#include "CombatStyle.h"
#include "GameplayTagContainer.h"
#include "UObject/Class.h"
#include "CombatStyleCombo.generated.h"

USTRUCT(BlueprintType)
struct NAZLACAN_API FCombatStyleCombo {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECombatStyle CombatStyle;

	// Attacks that become available by pressing Attack 1 after performing a specific attack.
	// Key: Previous Attack Tag, Value: Next Attack Tag
	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<FGameplayTag, FGameplayTag> Attack1Input = {};

	// Attacks that become available by pressing Attack 2 after performing a specific attack.
	// Key: Previous Attack Tag, Value: Next Attack Tag
	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<FGameplayTag, FGameplayTag> Attack2Input = {};

	// Attacks that become available by pressing Attack 3 after performing a specific attack.
	// Key: Previous Attack Tag, Value: Next Attack Tag
	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<FGameplayTag, FGameplayTag> Attack3Input = {};

	UPROPERTY(EditAnywhere, Category = "Combat")
	FGameplayTag AerialAttack = FGameplayTag::EmptyTag;
};
