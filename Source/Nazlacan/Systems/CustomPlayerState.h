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

public:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FDataTableRowHandle DefaultWeapons[2];

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

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(const FDataTableRowHandle& WeaponRowHandle, uint8 HandIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapons(const FDataTableRowHandle& RightHandWeapon, const FDataTableRowHandle& LeftHandWeapon);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveWeapon(uint8 HandIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	AWeapon* GetEquippedWeapon(const uint8 HandIndex) const { return EquippedWeapons[HandIndex]; }

private:
	void UpdateCombatStyle();
};
