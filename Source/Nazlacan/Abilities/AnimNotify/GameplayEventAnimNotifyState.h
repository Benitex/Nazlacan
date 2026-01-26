#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayEventAnimNotifyState.generated.h"

UCLASS()
class NAZLACAN_API UGameplayEventAnimNotifyState : public UAnimNotifyState {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StartEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EndEvent;

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
	virtual void ActivateEvent(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag Tag);
};
