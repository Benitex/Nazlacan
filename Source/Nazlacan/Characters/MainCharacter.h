#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class NAZLACAN_API AMainCharacter : public ACharacter {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle RightHandWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle LeftHandWeapon;


	// How much to reduce the upward velocity after stopping a jump
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling",
	meta = (ClampMin = 0, ClampMax = 1, AllowPrivateAccess = "true"))
	float JumpBreakMultiplier = 0;

public:
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void StopJumping() override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupCamera();
};
