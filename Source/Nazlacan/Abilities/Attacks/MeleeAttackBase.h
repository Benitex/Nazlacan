#pragma once

#include "CoreMinimal.h"
#include "Nazlacan/Abilities/BaseGameplayAbility.h"
#include "MeleeAttackBase.generated.h"

UCLASS()
class NAZLACAN_API UMeleeAttackBase : public UBaseGameplayAbility {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UGameplayEffect> EffectToApplyOnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (ClampMin = "0.0", ClampMax = "3.0"))
	float MotionValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (ClampMin = "0.0", ClampMax = "3.0"))
	float AnimationBaseSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (ClampMin = "0.0", ClampMax = "3.0"))
	float SpeedModifier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bShouldApplyElementalDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bUsesRightHandWeapon = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bUsesLeftHandWeapon = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool bShouldContinueCombo = false;

protected:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void TryToActivateNextAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StartHitDetection() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StopHitDetection() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual bool ShouldMoveDuringAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void RemoveLastAttackTag() const;

private:
	FGameplayEffectSpecHandle GetEffectSpecHandle(const uint8 ForHand) const;
};
