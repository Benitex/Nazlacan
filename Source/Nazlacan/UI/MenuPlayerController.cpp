#include "MenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Nazlacan/Macros.h"

void AMenuPlayerController::BeginPlay() {
    Super::BeginPlay();
    LoadMenu();
    LoadInputMode();
}

void AMenuPlayerController::LoadMenu() {
    returnIfNull(MenuWidget);

    Menu = CreateWidget<UUserWidget>(this, MenuWidget);
    returnIfNull(Menu);
    Menu->AddToViewport();
}

void AMenuPlayerController::LoadInputMode() {
    returnIfNull(Menu);

    bShowMouseCursor = true;

    FInputModeUIOnly InputMode;    
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetWidgetToFocus(Menu->TakeWidget());
    SetInputMode(InputMode);
}
