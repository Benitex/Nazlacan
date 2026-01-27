#include "Weapon.h"
#include "Nazlacan/Macros.h"

AWeapon* AWeapon::Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, APawn* NewOwner) {
	AWeapon* SpawnedWeapon = NewOwner->GetWorld()->SpawnActorDeferred<AWeapon>(
		StaticClass(),
		SpawnPosition,
		NewOwner,
		NewOwner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	returnIfNull(SpawnedWeapon) nullptr;
	return InternalSpawn(SpawnedWeapon, WeaponData, SpawnPosition);
}

AWeapon* AWeapon::Spawn(const FWeaponData& WeaponData, const FTransform& SpawnPosition, UWorld* World) {
	AWeapon* SpawnedWeapon = World->SpawnActorDeferred<AWeapon>(
		StaticClass(),
		SpawnPosition,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	returnIfNull(SpawnedWeapon) nullptr;
	return InternalSpawn(SpawnedWeapon, WeaponData, SpawnPosition);
}

AWeapon* AWeapon::InternalSpawn(AWeapon* SpawnedWeapon, const FWeaponData& WeaponData, const FTransform& SpawnPosition) {
	SpawnedWeapon->WeaponDataTable = WeaponData;
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
	if (!WeaponDataTable.WeaponMesh) {
		UE_LOG(LogTemp, Warning, TEXT("WeaponDataTable does not have a valid WeaponMesh assigned."));
		return;
	}
	WeaponMesh->SetStaticMesh(WeaponDataTable.WeaponMesh);

	FVector Min, Max;
	WeaponMesh->GetLocalBounds(Min, Max);

	FVector BoxExtent = (Max - Min) / 2;
	BoxExtent.Z += CollisionBoxZPadding;

	FVector BoxCenter = (Min + Max) / 2;
	BoxCenter.Z += CollisionBoxZPadding / 2;

	CollisionBox->SetBoxExtent(BoxExtent);
	CollisionBox->SetRelativeLocation(BoxCenter);
}

void AWeapon::StartCollisionDetection(AActor* Attacker) {
	IgnoredActors = {Attacker};
	HitActors.Empty();
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
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
	if (IgnoredActors.Contains(OtherActor)) return;
	HitActors.Add(OtherActor);
}
