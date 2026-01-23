#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitiesAttributeSet.generated.h"

UCLASS()
class NAZLACAN_API UAbilitiesAttributeSet : public UAttributeSet {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData SprintSpeed;

public:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS_BASIC(UAbilitiesAttributeSet, SprintSpeed);
	UFUNCTION() virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
