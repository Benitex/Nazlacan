#include "CustomGameMode.h"
#include "Nazlacan/Characters/PlayerCharacters/MainCharacter.h"
#include "Corruption/EnemySpawner.h"
#include "Nazlacan/Macros.h"

void ACustomGameMode::RestartPlayer(AController* NewPlayer) {
    Super::RestartPlayer(NewPlayer);

    AMainCharacter* MainCharacter = Cast<AMainCharacter>(NewPlayer->GetCharacter());
    returnIfNull(MainCharacter);

    MainCharacter->AddComponentByClass(UEnemySpawner::StaticClass(), false, FTransform(), false);
}
