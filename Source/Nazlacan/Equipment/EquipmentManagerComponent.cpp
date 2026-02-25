#include "EquipmentManagerComponent.h"
#include "Nazlacan/Macros.h"

void UEquipmentManagerComponent::EquipItem(const TScriptInterface<IEquipment> Item, const EEquipmentSlot ToSlot) {
	if (Item.GetObject() == EquippedItems[ToSlot].GetObject()) return;
	RemoveItem(ToSlot);
	EquippedItems.Add(ToSlot, Item);
}

void UEquipmentManagerComponent::RemoveItem(const EEquipmentSlot Slot) {
	const TScriptInterface<IEquipment>* Equipment = EquippedItems.Find(Slot);
	if (!Equipment || !IsValid(Equipment->GetObject())) return;

	Equipment->GetInterface()->DestroyEquipment();
	EquippedItems.Remove(Slot);
}

void UEquipmentManagerComponent::EquipWeapons(AWeapon* RightHandWeapon, AWeapon* LeftHandWeapon) {
	returnIfNull(RightHandWeapon);

	if (IsValid(RightHandWeapon)) {
		EquipItem(RightHandWeapon, EEquipmentSlot::RightHand);
	}

	if (IsValid(LeftHandWeapon)) {
		EquipItem(LeftHandWeapon, EEquipmentSlot::LeftHand);
	} else {
		RemoveItem(EEquipmentSlot::LeftHand);
	}
}

AWeapon* UEquipmentManagerComponent::GetEquippedWeapon(const EEquipmentSlot HandSlot) const {
	if (HandSlot != EEquipmentSlot::RightHand && HandSlot != EEquipmentSlot::LeftHand) {
		UE_LOG(LogTemp, Error, TEXT("Invalid hand slot specified for GetEquippedWeapon: %d"), static_cast<uint8>(HandSlot));
		return nullptr;
	}

	const TScriptInterface<IEquipment>* Equipment = EquippedItems.Find(HandSlot);
	if (!Equipment || !IsValid(Equipment->GetObject())) {
		return nullptr;
	}

	return Cast<AWeapon>(Equipment->GetObject());
}

TOptional<ECombatStyle> UEquipmentManagerComponent::GetCombatStyle() const {
	const AWeapon* RightHandWeapon = GetEquippedWeapon(EEquipmentSlot::RightHand);
	returnIfNull(RightHandWeapon) TOptional<ECombatStyle>();
    const EWeaponType RightHandWeaponType = RightHandWeapon->GetWeaponData().WeaponType;

    EWeaponType LeftHandWeaponType = EWeaponType::None;
	if (const AWeapon* LeftHandWeapon = GetEquippedWeapon(EEquipmentSlot::LeftHand); IsValid(LeftHandWeapon)) {
        LeftHandWeaponType = LeftHandWeapon->GetWeaponData().WeaponType;
    }

    if (RightHandWeaponType == EWeaponType::Sword && LeftHandWeaponType == EWeaponType::SpellFocus) {
        return ECombatStyle::SwordAndSorcery;
    } else if (RightHandWeaponType == EWeaponType::Sword && LeftHandWeaponType == EWeaponType::Sword) {
        return ECombatStyle::DualWielding;
    } else if (RightHandWeaponType == EWeaponType::Sword && LeftHandWeaponType == EWeaponType::None) {
        return ECombatStyle::SingleSword;
    } else if (RightHandWeaponType == EWeaponType::Heavy) {
        return ECombatStyle::TwoHanded;
    } else if (RightHandWeaponType == EWeaponType::SpellFocus && LeftHandWeaponType == EWeaponType::SpellFocus) {
        return ECombatStyle::Spellcasting;
    } else if (RightHandWeaponType == EWeaponType::Bow) {
        return ECombatStyle::Archery;
    }

	return TOptional<ECombatStyle>();
}
