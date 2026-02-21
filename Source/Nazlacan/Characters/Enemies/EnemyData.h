#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Nazlacan/Systems/Corruption/Sun.h"
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct NAZLACAN_API FEnemyData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESun Sun = ESun::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalChance = 0.05;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExperienceDroppedOnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AController> Controller;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UPhysicsAsset> PhysicsAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	float CapsuleHalfHeight = 88;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	float CapsuleRadius = 34;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	FTransform MeshOffsetTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimationInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitReactMontage;
};
