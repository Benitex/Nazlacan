#include "CustomGameInstance.h"
#include "MoviePlayer.h"
#include "Nazlacan/UI/SLoadingScreen.h"

void UCustomGameInstance::Init() {
    Super::Init();
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UCustomGameInstance::StartLoadingScreen);
}

void UCustomGameInstance::StartLoadingScreen(const FString& MapName) {
    if (IsRunningDedicatedServer()) return;

    FLoadingScreenAttributes LoadingScreen;
    LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
    LoadingScreen.bWaitForManualStop = false;
    LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen);

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}
