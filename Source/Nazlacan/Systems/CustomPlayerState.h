#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Nazlacan/Weapons/CombatStyle.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "CustomPlayerState.generated.h"

UCLASS()
class NAZLACAN_API ACustomPlayerState : public APlayerState {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	int32 MaxEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 Energy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
    int32 StartingPotionCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 PotionCount;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FDataTableRowHandle DefaultWeapons[2];

	static constexpr uint8 RightHandIndex = 0;
	static constexpr uint8 LeftHandIndex = 1;

protected:
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapons[2];

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	ECombatStyle CombatStyle = ECombatStyle::SwordAndSorcery;

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(FDataTableRowHandle& WeaponRowHandle, uint8 HandIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapons(FDataTableRowHandle& RightHandWeapon, FDataTableRowHandle& LeftHandWeapon);

	UFUNCTION(BLueprintCallable, Category = "Combat")
	void EquipDefaultWeapons();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveWeapon(uint8 HandIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	ECombatStyle GetCombatStyle() const { return CombatStyle; }

private:
	void UpdateCombatStyle();
};
