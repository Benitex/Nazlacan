#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

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

	CharacterAttributeSet->SetMaxHealth(Data.Health);

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
}
