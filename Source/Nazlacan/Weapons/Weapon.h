#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Weapon.generated.h"

UCLASS()
class NAZLACAN_API AWeapon : public AActor {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponData WeaponDataTable;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CorruptionIntensity = 0;

private:
	TSet<TWeakObjectPtr<AActor>> IgnoredActors = {};
	TSet<TWeakObjectPtr<AActor>> HitActors = {};

	static constexpr float CollisionBoxZPadding = 10;

public:
	UFUNCTION(BlueprintCallable)
	static AWeapon* Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, APawn* NewOwner);
	static AWeapon* Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, UWorld* World);

	AWeapon();

	FWeaponData GetWeaponData() const { return WeaponDataTable; }

	void StartCollisionDetection(AActor* Attacker);
	void StopCollisionDetection() const;
	TSet<TWeakObjectPtr<AActor>> GetHitActors() const { return HitActors; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	static AWeapon* InternalSpawn(AWeapon* SpawnedWeapon, const FWeaponData& WeaponData, const FTransform& SpawnPosition);
};
