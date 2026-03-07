#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "EnemyData.h"
#include "Nazlacan/Characters/BaseCharacter.h"
#include "Nazlacan/Characters/AttributeSets/CharacterAttributeSet.h"
#include "Enemy.generated.h"

UCLASS()
class NAZLACAN_API AEnemy : public ACharacter, public IBaseCharacter, public IAbilitySystemInterface {
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCharacterAttributeSet> CharacterAttributeSet;

	UPROPERTY(EditAnywhere)
	FEnemyData Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CorruptionIntensity;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

public:
	AEnemy();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual float GetBaseDamage() const override { return Data.BaseDamage; }

	virtual ESun GetDominantSun() const override { return Data.Sun; }
	virtual float GetDefaultCriticalChance() const override { return Data.CriticalChance; }
	virtual float GetCorruptionPercent() const override { return CorruptionIntensity; }
	virtual float GetExperienceDroppedOnDeath() const override { return Data.ExperienceDroppedOnDeath; }

	virtual UAnimMontage* GetDeathMontage() const override { return Data.DeathMontage; }
	virtual UAnimMontage* GetHitReactMontage() const override { return Data.HitReactMontage; }
};
