#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainCharacter.generated.h"

UCLASS()
class NAZLACAN_API AMainCharacter : public ACharacter {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (ClampMin = 0, Units = "cm", AllowPrivateAccess = "true"))
	float CameraDistance;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bShouldLagCameraOnMovement = false;

	// How much to reduce the upward velocity after stopping a jump
	UPROPERTY(EditAnywhere, Category = "Character Movement: Jumping / Falling", meta = (ClampMin = 0, ClampMax = 1, AllowPrivateAccess = "true"))
	float JumpBreakMultiplier = 0;

public:
	AMainCharacter();
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;
	virtual void StopJumping() override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupCamera();
};
