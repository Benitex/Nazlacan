#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class NAZLACAN_API AMainCharacter : public ACharacter {
    GENERATED_BODY()

    // How much to reduce the upward velocity after stopping a jump
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling",
    meta = (ClampMin = 0, ClampMax = 1, AllowPrivateAccess = "true"))
    float JumpBreakMultiplier = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Sprinting",
    meta = (ClampMin = 0, ForceUnits = "cm/s", AllowPrivateAccess = "true"))
    float MaxSprintingSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Sprinting",
    meta = (ClampMin = 0, ClampMax = 15, AllowPrivateAccess = "true"))
    float SprintDeceleration;

    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FName RightHandSocketName = TEXT("right_hand_socket");
    UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    FName LeftHandSocketName = TEXT("left_hand_socket");

private:
    FTransform RightHandSocket;
    FTransform LeftHandSocket;

    float MaxWalkSpeed; // Used to store the default speed set by the Character Movement Component

public:
    AMainCharacter();
    virtual void PostInitializeComponents() override;

    virtual void StopJumping() override;
    void StartSprinting() const;
    void StopSprinting() const;

    FName GetRightHandSocketName() const { return RightHandSocketName; }
    FName GetLeftHandSocketName() const { return LeftHandSocketName; }

private:
    void SetupCamera();
    void LoadSockets();
};
