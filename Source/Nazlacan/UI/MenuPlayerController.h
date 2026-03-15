#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

UCLASS()
class NAZLACAN_API AMenuPlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidget;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> Menu;

protected:
	virtual void BeginPlay() override;

private:
	void LoadMenu();
	void LoadInputMode();
};
