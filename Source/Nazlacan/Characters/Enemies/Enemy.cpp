#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Nazlacan/Macros.h"
#include "PhysicsEngine/PhysicsAsset.h"

AEnemy* AEnemy::Spawn(const FEnemyData& EnemyData, const float Corruption, const FTransform& SpawnPosition, UWorld* World) {
	AEnemy* SpawnedEnemy = World->SpawnActorDeferred<AEnemy>(StaticClass(), SpawnPosition);
	returnIfNull(SpawnedEnemy) nullptr;

	SpawnedEnemy->Data = EnemyData;
	SpawnedEnemy->CorruptionIntensity = Corruption;

	SpawnedEnemy->FinishSpawning(SpawnPosition);
	return SpawnedEnemy;
}

AEnemy::AEnemy() {
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

void AEnemy::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (Data.Health == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Possibly invalid enemy %s being created"), *Data.Name.ToString());
	}

	CharacterAttributeSet->InitMaxHealth(Data.Health);
	CharacterAttributeSet->InitHealth(Data.Health);
	GetCharacterMovement()->MaxWalkSpeed = Data.MovementSpeed;

	if (IsValid(Data.Mesh)) GetMesh()->SetSkeletalMesh(Data.Mesh);
	if (IsValid(Data.PhysicsAsset)) GetMesh()->SetPhysicsAsset(Data.PhysicsAsset);
	GetCapsuleComponent()->InitCapsuleSize(Data.CapsuleRadius, Data.CapsuleHalfHeight);
	GetMesh()->SetRelativeTransform(Data.MeshOffsetTransform);

	if (IsValid(Data.AnimationInstance)) GetMesh()->SetAnimInstanceClass(Data.AnimationInstance);
	if (IsValid(Data.Controller)) AIControllerClass = Data.Controller;
}

void AEnemy::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities) {
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
	}

	for (const TSubclassOf<UGameplayAbility>& Ability : Data.Abilities) {
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
	}
}
