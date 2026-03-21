#include "SLoadingScreen.h"
#include "SlateOptMacros.h"
#include "SlateExtras.h"
#include "Nazlacan/Macros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLoadingScreen::LoadBrushes() {
	BackgroundImage = LoadObject<UTexture2D>(nullptr, BackgroundImagePath.GetData());
	returnIfNull(BackgroundImage);
	BackgroundImage->AddToRoot();

	BackgroundImageBrush.SetResourceObject(BackgroundImage);
	BackgroundImageBrush.ImageSize = FVector2D(BackgroundImage->GetSizeX(), BackgroundImage->GetSizeY());

	TranslucentBlackEffectMaterial = LoadObject<UMaterialInterface>(nullptr, TranslucentBlackEffectMaterialPath.GetData());
	returnIfNull(TranslucentBlackEffectMaterial);
	TranslucentBlackEffectMaterial->AddToRoot();

	TranslucentBlackEffectBrush.SetResourceObject(TranslucentBlackEffectMaterial);
}

void SLoadingScreen::Construct(const FArguments& InArgs) {
	LoadBrushes();

	ChildSlot [
		SNew(SOverlay) +

		SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill) [
			SNew(SImage).Image(&BorderBrush)
		] +

		SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill) [
			SNew(SImage).Image(&BackgroundImageBrush)
		] +

		SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Bottom) [
			SNew(SBox).HeightOverride(832) [
				SNew(SImage).Image(&TranslucentBlackEffectBrush)
			]
		] +

		SOverlay::Slot().HAlign(HAlign_Right).VAlign(VAlign_Bottom).Padding(
			FMargin(0, 0, 56, 56)
		) [
			SNew(SCircularThrobber).NumPieces(8).Period(1.2).Radius(32)
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SLoadingScreen::~SLoadingScreen() {
	if (BackgroundImage && BackgroundImage->IsRooted()) {
		BackgroundImage->RemoveFromRoot();
	}

	if (TranslucentBlackEffectMaterial && TranslucentBlackEffectMaterial->IsRooted()) {
		TranslucentBlackEffectMaterial->RemoveFromRoot();
	}
}
