#include "Weapon.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Nazlacan/Macros.h"

AWeapon* AWeapon::Spawn(
	const FWeaponData& WeaponData, const float Corruption, const ESun Sun,
	const FTransform& SpawnPosition, APawn* NewOwner
) {
	AWeapon* SpawnedWeapon = NewOwner->GetWorld()->SpawnActorDeferred<AWeapon>(
		StaticClass(),
		SpawnPosition,
		NewOwner,
		NewOwner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	returnIfNull(SpawnedWeapon) nullptr;
	return InternalSpawn(SpawnedWeapon, WeaponData, Corruption, Sun, SpawnPosition);
}

AWeapon* AWeapon::Spawn(
	const FWeaponData& WeaponData, const float Corruption, const ESun Sun,
	const FTransform& SpawnPosition, UWorld* World
) {
	AWeapon* SpawnedWeapon = World->SpawnActorDeferred<AWeapon>(
		StaticClass(),
		SpawnPosition,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	returnIfNull(SpawnedWeapon) nullptr;
	return InternalSpawn(SpawnedWeapon, WeaponData, Corruption, Sun, SpawnPosition);
}

AWeapon* AWeapon::InternalSpawn(
	AWeapon* SpawnedWeapon,
	const FWeaponData& WeaponData,
	const float CorruptionIntensity,
	const ESun DominantSun,
	const FTransform& SpawnPosition
) {
	SpawnedWeapon->Data = WeaponData;

	SpawnedWeapon->DominantSun = DominantSun;
	if (SpawnedWeapon->Data.DefaultSun != ESun::None) {
		SpawnedWeapon->DominantSun = SpawnedWeapon->Data.DefaultSun;
	}

	SpawnedWeapon->CorruptionIntensity = CorruptionIntensity;
	SpawnedWeapon->FinishSpawning(SpawnPosition);
	return SpawnedWeapon;
}

AWeapon::AWeapon() {
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->UpdateBounds();

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlap);

	bReplicates = true;
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
	if (!Data.Mesh) {
		UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable does not have a valid Mesh assigned."));
		return;
	}
	WeaponMesh->SetStaticMesh(Data.Mesh);

	FVector Min, Max;
	WeaponMesh->GetLocalBounds(Min, Max);

	FVector BoxExtent = (Max - Min) / 2;
	BoxExtent.Z += CollisionBoxZPadding;
	CollisionBox->SetBoxExtent(BoxExtent);

	FVector BoxCenter = (Min + Max) / 2;
	BoxCenter.Z += CollisionBoxZPadding;
	CollisionBox->SetRelativeLocation(BoxCenter);
}

void AWeapon::StartCollisionDetection(AActor* Attacker, const FGameplayEffectSpecHandle& EffectToApplyOnHit) {
	IgnoredActors = {Attacker};
	HitActors.Empty();
	EffectToApplyOnNextHit = EffectToApplyOnHit;
	CollisionBox->SetCollisionProfileName(TEXT("WeaponAttack"));
}

void AWeapon::StopCollisionDetection() const {
	CollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
}

void AWeapon::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (!OtherActor || IgnoredActors.Contains(OtherActor)) return;
	const IAbilitySystemInterface* Target = Cast<IAbilitySystemInterface>(OtherActor);
	if (!Target) return;
	HitActors.Add(OtherActor);
	if (!EffectToApplyOnNextHit.IsValid()) return;

	Target->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectToApplyOnNextHit.Data.Get());
}
