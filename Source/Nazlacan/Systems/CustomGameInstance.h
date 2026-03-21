#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

UCLASS()
class NAZLACAN_API UCustomGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	virtual void Init() override;

private:
	UFUNCTION()
	virtual void StartLoadingScreen(const FString& MapName);
};
