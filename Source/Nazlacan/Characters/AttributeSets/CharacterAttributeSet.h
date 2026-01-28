#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

UCLASS()
class NAZLACAN_API UCharacterAttributeSet : public UAttributeSet {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS_BASIC(UCharacterAttributeSet, MaxHealth);
	UFUNCTION() virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	ATTRIBUTE_ACCESSORS_BASIC(UCharacterAttributeSet, Health);
	UFUNCTION() virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void GrantExperienceToKiller(const FGameplayEffectModCallbackData& Data);
};
