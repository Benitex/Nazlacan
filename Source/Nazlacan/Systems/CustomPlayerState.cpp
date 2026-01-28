#include "CustomPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Nazlacan/Macros.h"
#include "Nazlacan/Characters/MainCharacter.h"
#include "Nazlacan/Weapons/Weapon.h"
#include "Nazlacan/Weapons/WeaponData.h"

ACustomPlayerState::ACustomPlayerState() {
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
    PlayerAttributeSet = CreateDefaultSubobject<UPlayerCharacterAttributeSet>(TEXT("PlayerAttributeSet"));
    AbilityAttributeSet = CreateDefaultSubobject<UAbilitiesAttributeSet>(TEXT("AbilityAttributeSet"));

    EquippedWeapons[RightHandIndex] = nullptr;
    EquippedWeapons[LeftHandIndex] = nullptr;
}

void ACustomPlayerState::SetDefaultAbilitiesAndEffects() {
    if (GetLocalRole() != ROLE_Authority) return;

    EquipWeapons(DefaultWeapons[RightHandIndex], DefaultWeapons[LeftHandIndex]);

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

void ACustomPlayerState::EquipWeapon(const FDataTableRowHandle& WeaponRowHandle, const uint8 HandIndex) {
    returnIfNull(WeaponRowHandle.DataTable);
    AMainCharacter* MainCharacter = GetPawn<AMainCharacter>();
    returnIfNull(MainCharacter);
    USkeletalMeshComponent* Mesh = MainCharacter->GetMesh();
    returnIfNull(Mesh);

    RemoveWeapon(HandIndex);

    static const FString ContextString(TEXT("Equipping Weapon"));
    const FWeaponData* WeaponData = WeaponRowHandle.DataTable->FindRow<FWeaponData>(WeaponRowHandle.RowName, ContextString);
    returnIfNull(WeaponData);

    const FName SocketName = HandIndex == RightHandIndex ? MainCharacter->GetRightHandSocketName() : MainCharacter->GetLeftHandSocketName();
    const FTransform SocketTransform = Mesh->GetSocketTransform(SocketName);

    AWeapon* SpawnedWeapon = AWeapon::Spawn(*WeaponData, SocketTransform, MainCharacter);
    returnIfNull(SpawnedWeapon);

    SpawnedWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
    EquippedWeapons[HandIndex] = SpawnedWeapon;
}

void ACustomPlayerState::EquipWeapons(const FDataTableRowHandle& RightHandWeapon, const FDataTableRowHandle& LeftHandWeapon) {
    if (RightHandWeapon.DataTable != nullptr && RightHandWeapon.RowName != NAME_None) {
        EquipWeapon(RightHandWeapon, RightHandIndex);
    } else {
        RemoveWeapon(RightHandIndex);
    }

    if (LeftHandWeapon.DataTable != nullptr && LeftHandWeapon.RowName != NAME_None) {
        EquipWeapon(LeftHandWeapon, LeftHandIndex);
    } else {
        RemoveWeapon(LeftHandIndex);
    }

    UpdateCombatStyle();
}

void ACustomPlayerState::RemoveWeapon(const uint8 HandIndex) {
    if (EquippedWeapons[HandIndex] == nullptr) return;

    EquippedWeapons[HandIndex]->Destroy();
    EquippedWeapons[HandIndex] = nullptr;
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
        UE_LOG(LogTemp, Warning, TEXT("Invalid weapon combination selected."));
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
