#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerCharacterAttributeSet.generated.h"

UCLASS()
class NAZLACAN_API UPlayerCharacterAttributeSet : public UAttributeSet {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxSkullCount)
	FGameplayAttributeData MaxSkullCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_SkullCount)
	FGameplayAttributeData SkullCount;

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS_BASIC(UPlayerCharacterAttributeSet, MaxEnergy);
	UFUNCTION() virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerCharacterAttributeSet, Energy);
	UFUNCTION() virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);

	ATTRIBUTE_ACCESSORS_BASIC(UPlayerCharacterAttributeSet, Level);
	UFUNCTION() virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);

	ATTRIBUTE_ACCESSORS_BASIC(UPlayerCharacterAttributeSet, MaxSkullCount);
	UFUNCTION() virtual void OnRep_MaxSkullCount(const FGameplayAttributeData& OldMaxSkullCount);
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerCharacterAttributeSet, SkullCount);
	UFUNCTION() virtual void OnRep_SkullCount(const FGameplayAttributeData& OldSkullCount);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
