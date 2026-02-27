#pragma once

#include "CoreMinimal.h"
#include "MeleeAttack.h"
#include "Nazlacan/Characters/PlayerCharacters/MainCharacter.h"
#include "MeleeWeaponAttack.generated.h"

UCLASS()
class NAZLACAN_API UMeleeWeaponAttack : public UMeleeAttack {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (ClampMin = "0.0", ClampMax = "3.0"))
	float MotionValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bUsesRightHandWeapon = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bUsesLeftHandWeapon = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool bShouldContinueCombo = false;

protected:
	TWeakObjectPtr<AMainCharacter> MainCharacter;

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	AWeapon* GetEquippedWeapon(const EEquipmentSlot InHand) const;
	FGameplayEffectSpecHandle GetDamageEffectSpecHandleForHand(const EEquipmentSlot Hand) const;

	virtual void StartHitDetection() const override;
	virtual void StopHitDetection() const override;

	virtual bool ShouldMoveDuringAttack() const override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void TryToActivateNextAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void RemoveLastAttackTag() const;
};
