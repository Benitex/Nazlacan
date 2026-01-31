#include "DamageEffectCalculation.h"
#include "Nazlacan/Abilities/AbilitiesAttributeSet.h"
#include "Nazlacan/Characters/AttributeSets/CharacterAttributeSet.h"
#include "Nazlacan/Characters/BaseCharacter.h"

// Helper struct to capture relevant attributes.
struct FCapturedAttributes {
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraCriticalChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(SunWeaknessMultiplier);

    FCapturedAttributes() {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, Health, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAbilitiesAttributeSet, ExtraCriticalChance, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAbilitiesAttributeSet, SunWeaknessMultiplier, Source, true);
    }
};

// Singleton to access captured attributes.
static const FCapturedAttributes& CapturedAttributes() {
    static FCapturedAttributes CapturedAttributes;
    return CapturedAttributes;
}

UDamageEffectCalculation::UDamageEffectCalculation() {
    RelevantAttributesToCapture.Add(CapturedAttributes().ExtraCriticalChanceDef);
    RelevantAttributesToCapture.Add(CapturedAttributes().SunWeaknessMultiplierDef);
}

void UDamageEffectCalculation::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const {
    Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const IBaseCharacter* Instigator = Cast<IBaseCharacter>(Spec.GetContext().GetInstigator());
    const IBaseCharacter* Target = Cast<IBaseCharacter>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    static const FGameplayTag SunTag = FGameplayTag::RequestGameplayTag(FName("Data.Sun"));
    static const FGameplayTag CriticalMultiplierTag = FGameplayTag::RequestGameplayTag(FName("Data.CriticalMultiplier"));

    float Damage = Spec.GetSetByCallerMagnitude(DamageTag);
    const ESun AttackSun = static_cast<ESun>(Spec.GetSetByCallerMagnitude(SunTag));
    const float CriticalMultiplier = Spec.GetSetByCallerMagnitude(CriticalMultiplierTag);

    float CriticalChance = 0;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        CapturedAttributes().ExtraCriticalChanceDef,
        EvaluationParameters, 
        CriticalChance
    );
    CriticalChance += Instigator->GetDefaultCriticalChance();

    if (FMath::RandRange(0.0f, 1.0f) < CriticalChance) {
        Damage *= CriticalMultiplier;
    }

    if (Target->GetDominantSun() != ESun::None && Target->GetDominantSun() == AttackSun) {
        float SunWeaknessMultiplier = 2;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
            CapturedAttributes().SunWeaknessMultiplierDef,
            EvaluationParameters,
            SunWeaknessMultiplier
        );

        Damage *= SunWeaknessMultiplier;
    }

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
        CapturedAttributes().HealthProperty, EGameplayModOp::Additive, -Damage
    ));
}
