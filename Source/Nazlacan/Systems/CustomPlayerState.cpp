#include "CustomPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/PlayerCharacters/MainCharacter.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "Nazlacan/Weapons/WeaponData.h"

ACustomPlayerState::ACustomPlayerState() {
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
    PlayerAttributeSet = CreateDefaultSubobject<UPlayerCharacterAttributeSet>(TEXT("PlayerAttributeSet"));
    AbilityAttributeSet = CreateDefaultSubobject<UAbilitiesAttributeSet>(TEXT("AbilityAttributeSet"));

    EquipmentManager = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManager"));
}

void ACustomPlayerState::LoadAbilitiesAndEffects() {
    if (GetLocalRole() != ROLE_Authority) return;

    if (!bLoadedDefaultAttributes) {
        EquipStartingWeapons();
        LoadDefaultEffectsAndAttributes();
        bLoadedDefaultAttributes = true;
    } else {
        AMainCharacter* MainCharacter = GetPawn<AMainCharacter>();
        MainCharacter->AttachEquipmentToMesh(EquipmentManager->GetEquippedWeapon(EEquipmentSlot::RightHand), EEquipmentSlot::RightHand);
        MainCharacter->AttachEquipmentToMesh(EquipmentManager->GetEquippedWeapon(EEquipmentSlot::LeftHand), EEquipmentSlot::LeftHand);
    }

    // Reset attributes
    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this);
    const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(SetAttributesOnRespawnEffect, 1, Context);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void ACustomPlayerState::LoadDefaultEffectsAndAttributes() {
    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this);

    for (const TSubclassOf<UGameplayEffect>& Effect : DefaultEffects) {
        const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, Context);
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
    }

    for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities) {
        AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
    }
    for (const TSubclassOf<UGameplayAbility>& Attack : Attacks) {
        AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Attack));
    }
}

void ACustomPlayerState::EquipStartingWeapons() {
    constexpr uint8 RightHand = 0;
    constexpr uint8 LeftHand = 1;

    if (!IsValid(StartingWeapons[RightHand].DataTable) || StartingWeapons[RightHand].RowName == NAME_None) {
        UE_LOG(LogTemp, Warning, TEXT("Invalid starting weapons selected. Skipping weapon equip."));
        return;
    }

    AWeapon* Weapons[2] = { nullptr, nullptr };
    TArray<uint8> HandIndices = { RightHand };
    if (IsValid(StartingWeapons[RightHand].DataTable) && StartingWeapons[RightHand].RowName != NAME_None) {
        HandIndices.Add(LeftHand);
    }

    for (const uint8 HandIndex : HandIndices) {
        const FDataTableRowHandle RowHandle = HandIndex == RightHand ? StartingWeapons[RightHand] : StartingWeapons[LeftHand];

        static const FString ContextString(TEXT("Equipping Weapon"));
        const FWeaponData* WeaponData = RowHandle.DataTable->FindRow<FWeaponData>(RowHandle.RowName, ContextString);

        if (!ensure(WeaponData)) continue;

        Weapons[HandIndex] = AWeapon::Spawn(
           *WeaponData,
           StartingWeaponsCorruption,
           GetRandomSun(),
           GetPawn()->GetActorTransform(),
           GetPawn()
        );
    }

    EquipWeapons(Weapons[RightHand], Weapons[LeftHand]);
}

void ACustomPlayerState::EquipWeapons(AWeapon* RightHandWeapon, AWeapon* LeftHandWeapon) {
    EquipmentManager->EquipWeapons(RightHandWeapon, LeftHandWeapon);
    UpdateCombatStyle();
    AMainCharacter* MainCharacter = GetPawn<AMainCharacter>();
    returnIfNull(MainCharacter);
    MainCharacter->AttachEquipmentToMesh(EquipmentManager->GetEquippedWeapon(EEquipmentSlot::RightHand), EEquipmentSlot::RightHand);
    MainCharacter->AttachEquipmentToMesh(EquipmentManager->GetEquippedWeapon(EEquipmentSlot::LeftHand), EEquipmentSlot::LeftHand);
}

void ACustomPlayerState::UpdateCombatStyle() {
    const ECombatStyle* NewCombatStyle = EquipmentManager->GetCombatStyle().GetPtrOrNull();
    if (!NewCombatStyle) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to determine Combat Style for player %s."), *GetPlayerName());
        return;
    }

    const ECombatStyle PreviousCombatStyle = CombatStyle;
    CombatStyle = *NewCombatStyle;

    static const TMap<ECombatStyle, FGameplayTag> CombatStyleTags = {
        {ECombatStyle::SwordAndSorcery, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Sword.SwordAndSorcery"))},
        {ECombatStyle::SingleSword, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Sword.SingleSword"))},
        {ECombatStyle::DualWielding, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Sword.DualWielding"))},
        {ECombatStyle::TwoHanded, FGameplayTag::RequestGameplayTag(FName("CombatStyle.TwoHanded"))},
        {ECombatStyle::Spellcasting, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Spellcasting"))},
        {ECombatStyle::Archery, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Archery"))}
    };

    AbilitySystemComponent->RemoveLooseGameplayTag(CombatStyleTags[PreviousCombatStyle]);
    AbilitySystemComponent->AddLooseGameplayTag(CombatStyleTags[CombatStyle]);
}

ESun ACustomPlayerState::GetDominantSun() const {
    const AWeapon* RightHandWeapon = EquipmentManager->GetEquippedWeapon(EEquipmentSlot::RightHand);
    const AWeapon* LeftHandWeapon = EquipmentManager->GetEquippedWeapon(EEquipmentSlot::LeftHand);

    if (!IsValid(LeftHandWeapon)) {
        return RightHandWeapon->GetDominantSun();
    }

    if (RightHandWeapon->GetCorruptionIntensity() > LeftHandWeapon->GetCorruptionIntensity()) {
        return RightHandWeapon->GetDominantSun();
    } else {
        return LeftHandWeapon->GetDominantSun();
    }
}

float ACustomPlayerState::GetCorruptionPercent() const {
    float TotalCorruption = 0;

    if (const AWeapon* RightHandWeapon = EquipmentManager->GetEquippedWeapon(EEquipmentSlot::RightHand); IsValid(RightHandWeapon)) {
        TotalCorruption += RightHandWeapon->GetCorruptionIntensity();
    }

    if (const AWeapon* LeftHandWeapon = EquipmentManager->GetEquippedWeapon(EEquipmentSlot::LeftHand); IsValid(LeftHandWeapon)) {
        TotalCorruption += LeftHandWeapon->GetCorruptionIntensity();
    }

    return TotalCorruption;
}
