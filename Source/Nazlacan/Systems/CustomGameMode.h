#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

UCLASS()
class NAZLACAN_API ACustomGameMode : public AGameModeBase {
	GENERATED_BODY()

protected:
	virtual void RestartPlayer(AController* NewPlayer) override;
};
