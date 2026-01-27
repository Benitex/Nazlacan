#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Nazlacan/Systems/CustomPlayerState.h"
#include "MainCharacter.generated.h"

UCLASS()
class NAZLACAN_API AMainCharacter : public ACharacter, public IAbilitySystemInterface {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FName RightHandSocketName = TEXT("right_hand_socket");
    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FName LeftHandSocketName = TEXT("left_hand_socket");

protected:
    // How much to reduce the upward velocity after stopping a jump.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling", meta = (ClampMin = 0, ClampMax = 1))
    float JumpBreakMultiplier = 0;

private:
    TWeakObjectPtr<ACustomPlayerState> State;

    UPROPERTY(VisibleInstanceOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FGameplayTag LastAttack = FGameplayTag::EmptyTag;
    UPROPERTY(VisibleInstanceOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FGameplayTag NextAttack = FGameplayTag::EmptyTag;

    UPROPERTY(VisibleInstanceOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
    FVector MovementIntendedDirection;
    float DefaultMaxWalkSpeed;

    FTransform RightHandSocket;
    FTransform LeftHandSocket;

public:
    AMainCharacter();
    virtual void PostInitializeComponents() override;

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    virtual void StopJumping() override;

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Sprinting")
    void StartSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Sprinting")
    void StopSprinting() const;

    UFUNCTION(BlueprintCallable)
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
        return State->GetAbilitySystemComponent();
    }

    UFUNCTION(BlueprintCallable, Category = "Combat")
    ECombatStyle GetCombatStyle() const { return State->GetCombatStyle(); }

    UFUNCTION(BlueprintCallable)
    void ActivateAbilityWithTag(const FGameplayTag& Tag) const;

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

    AWeapon* GetEquippedWeapon(const uint8 HandIndex) const { return State->GetEquippedWeapon(HandIndex); }

    float GetDefaultMaxWalkSpeed() const { return DefaultMaxWalkSpeed; }
    FName GetRightHandSocketName() const { return RightHandSocketName; }
    FName GetLeftHandSocketName() const { return LeftHandSocketName; }

private:
    void SetupCamera();
    void LoadSockets();
};
