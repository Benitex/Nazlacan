#include "Weapon.h"

#include "Nazlacan/Macros.h"

AWeapon* AWeapon::Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, APawn* NewOwner) {
	AWeapon* SpawnedWeapon = NewOwner->GetWorld()->SpawnActorDeferred<AWeapon>(
		StaticClass(),
		SpawnPosition,
		NewOwner,
		NewOwner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	returnIfNull(SpawnedWeapon) nullptr;
	return InternalSpawn(SpawnedWeapon, WeaponData, SpawnPosition);
}

AWeapon* AWeapon::Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, UWorld* World) {
	AWeapon* SpawnedWeapon = World->SpawnActorDeferred<AWeapon>(
		StaticClass(),
		SpawnPosition,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	returnIfNull(SpawnedWeapon) nullptr;
	return InternalSpawn(SpawnedWeapon, WeaponData, SpawnPosition);
}

AWeapon* AWeapon::InternalSpawn(AWeapon* SpawnedWeapon, const FWeaponData& WeaponData, const FTransform& SpawnPosition) {
	SpawnedWeapon->WeaponDataTable = WeaponData;
	SpawnedWeapon->FinishSpawning(SpawnPosition);
	return SpawnedWeapon;
}

AWeapon::AWeapon() {
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
	if (!WeaponDataTable.WeaponMesh) {
		UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable does not have a valid WeaponMesh assigned."));
		return;
	}
	WeaponMesh->SetStaticMesh(WeaponDataTable.WeaponMesh);
}
