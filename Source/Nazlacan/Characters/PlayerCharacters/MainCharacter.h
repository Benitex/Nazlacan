#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Nazlacan/Characters/BaseCharacter.h"
#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"
#include "NavigationInvokerComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Nazlacan/Systems/CustomPlayerState.h"
#include "MainCharacter.generated.h"

UCLASS()
class NAZLACAN_API AMainCharacter : public ACharacter, public IBaseCharacter, public IAbilitySystemInterface {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNavigationInvokerComponent> NavigationInvoker;

    // Base damage for abilities that do not use Weapons
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float BaseDamage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0, ClampMax = 1, AllowPrivateAccess = "true"))
    float CriticalChance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = 0, AllowPrivateAccess = "true"))
    float HealingPower;

    UPROPERTY(EditDefaultsOnly)
    TMap<ECombatStyle, UAnimMontage*> HitReactMontages;
    UPROPERTY(EditDefaultsOnly)
    TMap<ECombatStyle, UAnimMontage*> DeathMontages;

    UPROPERTY(EditDefaultsOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
    TMap<EEquipmentSlot, FName> EquipmentSockets = {
        { EEquipmentSlot::RightHand, TEXT("right_hand_socket") },
        { EEquipmentSlot::LeftHand, TEXT("left_hand_socket") },
    };

protected:
    // How much to reduce the upward velocity after stopping a jump.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling", meta = (ClampMin = 0, ClampMax = 1))
    float JumpBreakMultiplier = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceDroppedOnDeath = 0;

private:
    TWeakObjectPtr<ACustomPlayerState> State;

    UPROPERTY(VisibleInstanceOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FGameplayTag LastAttack = FGameplayTag::EmptyTag;
    UPROPERTY(VisibleInstanceOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FGameplayTag NextAttack = FGameplayTag::EmptyTag;

    UPROPERTY(VisibleInstanceOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
    FVector MovementIntendedDirection;
    float DefaultMaxWalkSpeed;

public:
    AMainCharacter();
    virtual void PostInitializeComponents() override;

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    UFUNCTION(BlueprintCallable)
    virtual ACustomPlayerState* GetState() const { return State.Get(); }

    virtual float GetBaseDamage() const override { return BaseDamage; }
    virtual float GetDefaultCriticalChance() const override { return CriticalChance; }
    float GetDefaultHealingPower() const { return HealingPower; }

    UFUNCTION(BlueprintCallable, Category = "Corruption")
    virtual ESun GetDominantSun() const override { return State->GetDominantSun(); }

    UFUNCTION(BlueprintCallable, Category = "Corruption")
    virtual float GetCorruptionPercent() const override { return State->GetCorruptionPercent(); }

    virtual float GetExperienceDroppedOnDeath() const override { return ExperienceDroppedOnDeath; }
    virtual UAnimMontage* GetHitReactMontage() const override;
    virtual UAnimMontage* GetDeathMontage() const override { return DeathMontages[State->GetCombatStyle()]; }

    virtual void StopJumping() override;

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Sprinting")
    void StartSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Sprinting")
    void StopSprinting() const;

    UFUNCTION(BlueprintCallable)
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
        return State->GetAbilitySystemComponent();
    }

    UFUNCTION(BlueprintCallable)
    void ActivateEventToSelf(const FGameplayTag& Tag) const;

    UFUNCTION(BlueprintCallable)
    bool ActivateAbilityWithTag(const FGameplayTag& Tag) const;

    UFUNCTION(BlueprintCallable)
    void PrepareAttackWithTag(const FGameplayTag& Tag);
    void TryToActivateNextAttack();
    bool IsNextAttackPrepared() const { return NextAttack != FGameplayTag::EmptyTag; }

    void RemoveLastAttack(FGameplayTagContainer TagFilter = FGameplayTagContainer());
    FGameplayTag GetLastAttack() const { return LastAttack; }

    UFUNCTION(BlueprintCallable) bool IsAttacking() const;
    UFUNCTION(BlueprintCallable) bool CanMove() const;
    UFUNCTION(BlueprintCallable, Category = "Character Movement")
    bool IsFalling() const { return GetMovementComponent()->IsFalling(); }

    UFUNCTION(BlueprintCallable, Category = "Character Movement")
    FVector GetMovementIntendedDirection() const;
    void SetMovementIntendedDirection(const FVector& NewDirection) { MovementIntendedDirection = NewDirection; }

    float GetDefaultMaxWalkSpeed() const { return DefaultMaxWalkSpeed; }

    void AttachEquipmentToMesh(const TScriptInterface<IEquipment>& Equipment, const EEquipmentSlot Slot);

private:
    void SetupCamera();
};
