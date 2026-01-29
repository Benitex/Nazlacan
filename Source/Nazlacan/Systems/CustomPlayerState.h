#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Nazlacan/Abilities/AbilitiesAttributeSet.h"
#include "Nazlacan/Characters/AttributeSets/CharacterAttributeSet.h"
#include "Nazlacan/Characters/AttributeSets/PlayerCharacterAttributeSet.h"
#include "Nazlacan/Weapons/CombatStyle.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "CustomPlayerState.generated.h"

UCLASS()
class NAZLACAN_API ACustomPlayerState : public APlayerState, public IAbilitySystemInterface {
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY() TObjectPtr<const UCharacterAttributeSet> CharacterAttributeSet;
	UPROPERTY() TObjectPtr<const UPlayerCharacterAttributeSet> PlayerAttributeSet;
	UPROPERTY() TObjectPtr<const UAbilitiesAttributeSet> AbilityAttributeSet;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> Attacks;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ExperienceGrantEffect;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float StartingWeaponsCorruption;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FDataTableRowHandle StartingWeapons[2];

	static constexpr uint8 RightHandIndex = 0;
	static constexpr uint8 LeftHandIndex = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 Experience = 0;

protected:
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapons[2];

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	ECombatStyle CombatStyle = ECombatStyle::SwordAndSorcery;

public:
	ACustomPlayerState();

	// Only callable on server
	UFUNCTION(BlueprintCallable)
	void SetDefaultAbilitiesAndEffects();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	ECombatStyle GetCombatStyle() const { return CombatStyle; }

	UFUNCTION(BLueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComponent;
	}

	// Pass nullptr in LeftHandWeapon to only equip the right hand weapon and unequip the left hand weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapons(AWeapon* RightHandWeapon, AWeapon* LeftHandWeapon);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveLeftHandWeapon() { RemoveWeapon(LeftHandIndex); UpdateCombatStyle(); }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AWeapon* GetEquippedWeapon(const uint8 HandIndex) const { return EquippedWeapons[HandIndex]; }

	ESun GetDominantSun() const;
	float GetCorruptionPercent() const;

	TSubclassOf<UGameplayEffect> GetExperienceGrantEffect() const { return ExperienceGrantEffect; }

protected:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(AWeapon* Weapon, uint8 HandIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveWeapon(uint8 HandIndex);

private:
	void UpdateCombatStyle();
	void EquipStartingWeapons();
};
