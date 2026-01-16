#include "CustomPlayerState.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/MainCharacter.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "Nazlacan/Weapons/WeaponData.h"

void ACustomPlayerState::EquipWeapon(FDataTableRowHandle& WeaponRowHandle, const uint8 HandIndex) {
    returnIfNull(WeaponRowHandle.DataTable);
    AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetPawn());
    returnIfNull(MainCharacter);
    USkeletalMeshComponent* Mesh = MainCharacter->GetMesh();
    returnIfNull(Mesh);

    RemoveWeapon(HandIndex);

    static const FString ContextString(TEXT("Equipping Weapon"));
    const FWeaponData* WeaponData = WeaponRowHandle.DataTable->FindRow<FWeaponData>(WeaponRowHandle.RowName, ContextString);
    returnIfNull(WeaponData);

    const FName SocketName = HandIndex == RightHandIndex ? MainCharacter->GetRightHandSocketName() : MainCharacter->GetLeftHandSocketName();
    if (!Mesh->DoesSocketExist(SocketName)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to equip %s in Socket %s because it does not exist on mesh"), *WeaponRowHandle.RowName.ToString(), *SocketName.ToString());
        return;
    }
    const FTransform SocketTransform = Mesh->GetSocketTransform(SocketName);

    AWeapon* SpawnedWeapon = AWeapon::Spawn(*WeaponData, SocketTransform, MainCharacter);
    returnIfNull(SpawnedWeapon);

    SpawnedWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
    EquippedWeapons[HandIndex] = SpawnedWeapon;
}

void ACustomPlayerState::EquipWeapons(FDataTableRowHandle& RightHandWeapon, FDataTableRowHandle& LeftHandWeapon) {
    if (RightHandWeapon.DataTable != nullptr && RightHandWeapon.RowName != NAME_None) {
        EquipWeapon(RightHandWeapon, RightHandIndex);
    }
    if (LeftHandWeapon.DataTable != nullptr && LeftHandWeapon.RowName != NAME_None) {
        EquipWeapon(LeftHandWeapon, LeftHandIndex);
    }
}

void ACustomPlayerState::EquipDefaultWeapons() {
    EquipWeapons(DefaultWeapons[RightHandIndex], DefaultWeapons[LeftHandIndex]);
}

void ACustomPlayerState::RemoveWeapon(const uint8 HandIndex) {
    if (EquippedWeapons[HandIndex] == nullptr) return;

    EquippedWeapons[HandIndex]->Destroy();
    EquippedWeapons[HandIndex] = nullptr;
}

void ACustomPlayerState::UpdateCombatStyle() {
    // TODO update combat style according to the equipped weapons
}
