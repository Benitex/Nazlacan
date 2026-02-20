#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Nazlacan/Systems/Corruption/Sun.h"
#include "Weapon.generated.h"

UCLASS()
class NAZLACAN_API AWeapon : public AActor {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponData Data;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CorruptionIntensity = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Corruption")
	ESun DominantSun;

private:
	TSet<TWeakObjectPtr<AActor>> IgnoredActors = {};
	TSet<TWeakObjectPtr<AActor>> HitActors = {};
	FGameplayEffectSpecHandle EffectToApplyOnNextHit;

	static constexpr float CollisionBoxZPadding = 10;

public:
	UFUNCTION(BlueprintCallable)
	static AWeapon* Spawn(
		const FWeaponData& WeaponData, const float Corruption, const ESun Sun,
		const FTransform& SpawnPosition, APawn* NewOwner
	);

	static AWeapon* Spawn(
		const FWeaponData& WeaponData, const float Corruption, const ESun Sun,
		const FTransform& SpawnPosition, UWorld* World
	);

	AWeapon();

	FWeaponData GetWeaponData() const { return Data; }
	float GetCorruptionIntensity() const { return CorruptionIntensity; }
	ESun GetDominantSun() const { return DominantSun; }

	void StartCollisionDetection(AActor* Attacker, const FGameplayEffectSpecHandle& EffectToApplyOnHit);
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
	static AWeapon* InternalSpawn(
		AWeapon* SpawnedWeapon,
		const FWeaponData& WeaponData,
		const float CorruptionIntensity,
		const ESun DominantSun,
		const FTransform& SpawnPosition
	);
};
