#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class NAZLACAN_API AWeapon : public AActor {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponData WeaponDataTable;

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

public:
	UFUNCTION(BlueprintCallable)
	static AWeapon* Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, APawn* NewOwner);
	static AWeapon* Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, UWorld* World);

	AWeapon();

	FWeaponData GetWeaponData() const { return WeaponDataTable; }

protected:
	virtual void BeginPlay() override;

private:
	static AWeapon* InternalSpawn(AWeapon* SpawnedWeapon, const FWeaponData& WeaponData, const FTransform& SpawnPosition);
};
