#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
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

	UPROPERTY()
	TObjectPtr<const UCharacterAttributeSet> CharacterAttributeSet;
	UPROPERTY()
	TObjectPtr<const UPlayerCharacterAttributeSet> PlayerAttributeSet;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FDataTableRowHandle DefaultWeapons[2];

	static constexpr uint8 RightHandIndex = 0;
	static constexpr uint8 LeftHandIndex = 1;

	int32 Experience = 0;

protected:
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapons[2];

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	ECombatStyle CombatStyle = ECombatStyle::SwordAndSorcery;

public:
	ACustomPlayerState();

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

	UFUNCTION(BLueprintCallable, Category = "Combat")
	void EquipDefaultWeapons();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveWeapon(uint8 HandIndex);

private:
	void UpdateCombatStyle();
};
