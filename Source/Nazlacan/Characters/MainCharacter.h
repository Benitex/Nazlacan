#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Nazlacan/Systems/CustomPlayerState.h"
#include "MainCharacter.generated.h"

UCLASS()
class NAZLACAN_API AMainCharacter : public ACharacter, public IAbilitySystemInterface {
    GENERATED_BODY()

    // How much to reduce the upward velocity after stopping a jump
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling",
    meta = (ClampMin = 0, ClampMax = 1, AllowPrivateAccess = "true"))
    float JumpBreakMultiplier = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FName RightHandSocketName = TEXT("right_hand_socket");
    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FName LeftHandSocketName = TEXT("left_hand_socket");

private:
    TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    FTransform RightHandSocket;
    FTransform LeftHandSocket;

    float DefaultMaxWalkSpeed;

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

    UFUNCTION(BlueprintCallable, Category = "Character Movement: Dodging")
    void StartDodging() const;

    UFUNCTION(BlueprintCallable)
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {
        return AbilitySystemComponent.Get();
    }

    FName GetRightHandSocketName() const { return RightHandSocketName; }
    FName GetLeftHandSocketName() const { return LeftHandSocketName; }
    float GetDefaultMaxWalkSpeed() const { return DefaultMaxWalkSpeed; }

private:
    void SetupCamera();
    void LoadSockets();
    void LoadAbilitySystemComponent(ACustomPlayerState* FromState);
};
