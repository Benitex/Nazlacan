#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Components/ActorComponent.h"
#include "Nazlacan/Equipment/Weapons/CombatStyle.h"
#include "Nazlacan/Equipment/Weapons/Weapon.h"
#include "EquipmentManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8 {
	RightHand,
	LeftHand,
	Head,
	Heart,
	Liver,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChanged, EEquipmentSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponsEquipped);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NAZLACAN_API UEquipmentManagerComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnItemChanged OnItemChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponsEquipped OnWeaponsEquipped;

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

    // Use EquipWeapons for validated Weapon equips considering and updating Combat Styles
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(const TScriptInterface<IEquipment> Item, const EEquipmentSlot ToSlot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	TScriptInterface<IEquipment> GetItemInSlot(const EEquipmentSlot Slot);

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
