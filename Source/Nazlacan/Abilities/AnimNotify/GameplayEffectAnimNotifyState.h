#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayEffectAnimNotifyState.generated.h"

UCLASS()
class NAZLACAN_API UGameplayEffectAnimNotifyState : public UAnimNotifyState {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect;

public:
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

private:
	UAbilitySystemComponent* GetAbilitySystem(const USkeletalMeshComponent* FromMesh);
};
