#pragma once

#include "CoreMinimal.h"
#include "Nazlacan/Abilities/BaseGameplayAbility.h"
#include "Nazlacan/Equipment/EquipmentManagerComponent.h"
#include "MeleeAttack.generated.h"

UCLASS()
class NAZLACAN_API UMeleeAttack : public UBaseGameplayAbility {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UGameplayEffect> EffectToApplyOnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (ClampMin = "0.0", ClampMax = "3.0"))
	float AnimationBaseSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (ClampMin = "0.0", ClampMax = "3.0"))
	float SpeedModifier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bShouldApplyElementalDamage = false;

	static constexpr float DefaultCriticalDamageMultiplier = 2;

protected:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StartHitDetection() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StopHitDetection() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual bool ShouldMoveDuringAttack() const;

	FGameplayEffectSpecHandle GetDamageEffectSpecHandle(
		const float Damage,
		const ESun Sun,
		const float CriticalDamageMultiplier,
		AActor* EffectCauser = nullptr
	) const;
};
