#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Components/ActorComponent.h"
#include "Nazlacan/Weapons/CombatStyle.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "EquipmentManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8 {
	RightHand,
	LeftHand,
	Head,
	Heart,
	Liver,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAZLACAN_API UEquipmentManagerComponent : public UActorComponent {
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Equipment")
	TMap<EEquipmentSlot, TScriptInterface<IEquipment>> EquippedItems = {
		{ EEquipmentSlot::RightHand, nullptr },
		{ EEquipmentSlot::LeftHand, nullptr },
		{ EEquipmentSlot::Head, nullptr },
		{ EEquipmentSlot::Heart, nullptr },
		{ EEquipmentSlot::Liver, nullptr }
	};

public:
	UEquipmentManagerComponent() = default;

    // Use EquipWeapons for validated Weapon equips considering Combat Styles
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(const TScriptInterface<IEquipment> Item, const EEquipmentSlot ToSlot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void RemoveItem(const EEquipmentSlot Slot);

	// Pass nullptr in LeftHandWeapon to only equip the right hand weapon and unequip the left hand weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapons(AWeapon* RightHandWeapon, AWeapon* LeftHandWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeapon* GetEquippedWeapon(const EEquipmentSlot HandSlot) const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TOptional<ECombatStyle> GetCombatStyle() const;
};
