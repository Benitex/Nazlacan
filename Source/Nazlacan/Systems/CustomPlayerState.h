#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Nazlacan/Abilities/AbilitiesAttributeSet.h"
#include "Nazlacan/Characters/AttributeSets/CharacterAttributeSet.h"
#include "Nazlacan/Characters/AttributeSets/PlayerCharacterAttributeSet.h"
#include "Nazlacan/Equipment/EquipmentManagerComponent.h"
#include "Nazlacan/Weapons/CombatStyle.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "CustomPlayerState.generated.h"

UCLASS()
class NAZLACAN_API ACustomPlayerState : public APlayerState, public IAbilitySystemInterface {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentManagerComponent> EquipmentManager;

	UPROPERTY() TObjectPtr<const UCharacterAttributeSet> CharacterAttributeSet;
	UPROPERTY() TObjectPtr<const UPlayerCharacterAttributeSet> PlayerAttributeSet;
	UPROPERTY() TObjectPtr<const UAbilitiesAttributeSet> AbilityAttributeSet;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> Attacks;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> SetAttributesOnRespawnEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ExperienceGrantEffect;

	UPROPERTY(EditAnywhere, Category = "Corruption")
	float StartingWeaponsCorruption;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 Experience = 0;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FDataTableRowHandle StartingWeapons[2];

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	ECombatStyle CombatStyle = ECombatStyle::SwordAndSorcery;

private:
	bool bLoadedDefaultAttributes = false;

public:
	ACustomPlayerState();

	// Only callable on server
	void LoadAbilitiesAndEffects();
	void LoadDefaultEffectsAndAttributes();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	ECombatStyle GetCombatStyle() const { return CombatStyle; }

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComponent;
	}

	UFUNCTION(BlueprintCallable, Category = "Corruption")
	ESun GetDominantSun() const;

	UFUNCTION(BlueprintCallable, Category = "Corruption")
	float GetCorruptionPercent() const;

	TSubclassOf<UGameplayEffect> GetExperienceGrantEffect() const { return ExperienceGrantEffect; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool HasFullHealth() const { return CharacterAttributeSet->GetHealth() >= CharacterAttributeSet->GetMaxHealth(); }

	// Equips weapons using the EquipmentManagerComponent, updates the combat style and attaches the weapons to the character mesh.
	// Pass nullptr in LeftHandWeapon to only equip the right hand weapon and unequip the left hand weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapons(AWeapon* RightHandWeapon, AWeapon* LeftHandWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeapon* GetEquippedWeapon(const EEquipmentSlot HandSlot) const { return EquipmentManager->GetEquippedWeapon(HandSlot); }

private:
	void UpdateCombatStyle();
	void EquipStartingWeapons();
};
