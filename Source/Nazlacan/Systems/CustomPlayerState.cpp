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
}

void ACustomPlayerState::LoadAbilitiesAndEffects() {
    if (GetLocalRole() != ROLE_Authority) return;

    if (!bLoadedDefaultAttributes) {
        EquipStartingWeapons();
        LoadDefaultEffectsAndAttributes();
        bLoadedDefaultAttributes = true;
    } else {
        // Attach weapons to the mesh
        EquipWeapons(EquippedWeapons[RightHandIndex], EquippedWeapons[LeftHandIndex]);
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
    if (!IsValid(StartingWeapons[RightHandIndex].DataTable) || StartingWeapons[RightHandIndex].RowName == NAME_None) {
        return;
    }

    TArray<uint8> HandIndices = { RightHandIndex };
    if (IsValid(StartingWeapons[RightHandIndex].DataTable) && StartingWeapons[RightHandIndex].RowName != NAME_None) {
        HandIndices.Add(LeftHandIndex);
    }

    AMainCharacter* MainCharacter = GetPawn<AMainCharacter>();
    returnIfNull(MainCharacter);
    const USkeletalMeshComponent* Mesh = MainCharacter->GetMesh();
    returnIfNull(Mesh);

    for (const uint8 HandIndex : HandIndices) {
        const FDataTableRowHandle RowHandle = HandIndex == RightHandIndex ? StartingWeapons[RightHandIndex] : StartingWeapons[LeftHandIndex];

        static const FString ContextString(TEXT("Equipping Weapon"));
        const FWeaponData* WeaponData = RowHandle.DataTable->FindRow<FWeaponData>(RowHandle.RowName, ContextString);
        if (!ensure(WeaponData)) continue;

        const FName SocketName = HandIndex == RightHandIndex ? MainCharacter->GetRightHandSocketName() : MainCharacter->GetLeftHandSocketName();
        const FTransform SocketTransform = Mesh->GetSocketTransform(SocketName);

        AWeapon* Weapon = AWeapon::Spawn(*WeaponData, StartingWeaponsCorruption, GetRandomSun(), SocketTransform, MainCharacter);
        continueIfNull(Weapon);
        EquipWeapon(Weapon, HandIndex);
    }

    UpdateCombatStyle();
}

void ACustomPlayerState::EquipWeapons(AWeapon* RightHandWeapon, AWeapon* LeftHandWeapon) {
    returnIfNull(RightHandWeapon);

    if (IsValid(RightHandWeapon)) {
        EquipWeapon(RightHandWeapon, RightHandIndex);
    }

    if (IsValid(LeftHandWeapon)) {
        EquipWeapon(LeftHandWeapon, LeftHandIndex);
    } else {
        RemoveWeapon(LeftHandIndex);
    }

    UpdateCombatStyle();
}

void ACustomPlayerState::EquipWeapon(AWeapon* Weapon, const uint8 HandIndex) {
    const AMainCharacter* MainCharacter = GetPawn<AMainCharacter>();
    returnIfNull(MainCharacter);
    USkeletalMeshComponent* Mesh = MainCharacter->GetMesh();
    returnIfNull(Mesh);

    const FName SocketName = HandIndex == RightHandIndex ? MainCharacter->GetRightHandSocketName() : MainCharacter->GetLeftHandSocketName();

    if (Weapon != EquippedWeapons[HandIndex]) RemoveWeapon(HandIndex);
    Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
    EquippedWeapons[HandIndex] = Weapon;
}

void ACustomPlayerState::RemoveWeapon(const uint8 HandIndex) {
    if (!IsValid(EquippedWeapons[HandIndex])) return;
    EquippedWeapons[HandIndex]->Destroy();
    EquippedWeapons[HandIndex] = nullptr;
    UpdateCombatStyle();
}

void ACustomPlayerState::UpdateCombatStyle() {
    returnIfNull(EquippedWeapons[RightHandIndex]);
    const EWeaponType RightHandWeaponType = EquippedWeapons[RightHandIndex]->GetWeaponData().WeaponType;

    EWeaponType LeftHandWeaponType = EWeaponType::None;
    if (IsValid(EquippedWeapons[LeftHandIndex])) {
        LeftHandWeaponType = EquippedWeapons[LeftHandIndex]->GetWeaponData().WeaponType;
    }

    if (RightHandWeaponType == EWeaponType::Sword && LeftHandWeaponType == EWeaponType::SpellFocus) {
        CombatStyle = ECombatStyle::SwordAndSorcery;
    } else if (RightHandWeaponType == EWeaponType::Sword && LeftHandWeaponType == EWeaponType::Sword) {
        CombatStyle = ECombatStyle::DualWielding;
    } else if (RightHandWeaponType == EWeaponType::Sword && LeftHandWeaponType == EWeaponType::None) {
        CombatStyle = ECombatStyle::SingleSword;
    } else if (RightHandWeaponType == EWeaponType::Heavy) {
        CombatStyle = ECombatStyle::TwoHanded;
    } else if (RightHandWeaponType == EWeaponType::SpellFocus && LeftHandWeaponType == EWeaponType::SpellFocus) {
        CombatStyle = ECombatStyle::Spellcasting;
    } else if (RightHandWeaponType == EWeaponType::Bow) {
        CombatStyle = ECombatStyle::Archery;
    } else {
        UE_LOG(LogTemp, Warning, TEXT("%s is using an invalid weapon combination."), *(GetPawn()->GetName()));
    }

    static const TMap<ECombatStyle, FGameplayTag> CombatStyleTags = {
        {ECombatStyle::SwordAndSorcery, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Sword.SwordAndSorcery"))},
        {ECombatStyle::SingleSword, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Sword.SingleSword"))},
        {ECombatStyle::DualWielding, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Sword.DualWielding"))},
        {ECombatStyle::TwoHanded, FGameplayTag::RequestGameplayTag(FName("CombatStyle.TwoHanded"))},
        {ECombatStyle::Spellcasting, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Spellcasting"))},
        {ECombatStyle::Archery, FGameplayTag::RequestGameplayTag(FName("CombatStyle.Archery"))}
    };
    static TArray<FGameplayTag> CombatStyles = {};
    if (CombatStyles.Num() == 0) CombatStyleTags.GenerateValueArray(CombatStyles);

    AbilitySystemComponent->RemoveLooseGameplayTags(FGameplayTagContainer::CreateFromArray(CombatStyles));
    AbilitySystemComponent->AddLooseGameplayTag(CombatStyleTags[CombatStyle]);
}

ESun ACustomPlayerState::GetDominantSun() const {
    if (!IsValid(EquippedWeapons[LeftHandIndex])) {
        return EquippedWeapons[RightHandIndex]->GetDominantSun();
    }

    if (EquippedWeapons[RightHandIndex]->GetCorruptionIntensity() > EquippedWeapons[LeftHandIndex]->GetCorruptionIntensity()) {
        return EquippedWeapons[RightHandIndex]->GetDominantSun();
    } else {
        return EquippedWeapons[LeftHandIndex]->GetDominantSun();
    }
}

float ACustomPlayerState::GetCorruptionPercent() const {
    float TotalCorruption = 0;

    if (IsValid(EquippedWeapons[RightHandIndex])) {
        TotalCorruption += EquippedWeapons[RightHandIndex]->GetCorruptionIntensity();
    }

    if (IsValid(EquippedWeapons[LeftHandIndex])) {
        TotalCorruption += EquippedWeapons[LeftHandIndex]->GetCorruptionIntensity();
    }

    return TotalCorruption;
}
