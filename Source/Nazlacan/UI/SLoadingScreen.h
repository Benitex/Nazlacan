#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class NAZLACAN_API SLoadingScreen : public SCompoundWidget {
private:
	FSlateColorBrush BorderBrush { FLinearColor::Black };
	FSlateBrush BackgroundImageBrush;
	FSlateBrush TranslucentBlackEffectBrush;

	FStringView BackgroundImagePath = TEXT("/Game/Nazlacan/UI/Menus/LoadingScreen/luis-aceves-3bENUVe9MWY-unsplash.luis-aceves-3bENUVe9MWY-unsplash");
	FStringView TranslucentBlackEffectMaterialPath = TEXT("/Game/Nazlacan/UI/Widgets/Gradient/MI_BlackToTransparentGradient.MI_BlackToTransparentGradient");

	UTexture2D* BackgroundImage = nullptr;
	UMaterialInterface* TranslucentBlackEffectMaterial = nullptr;

public:
	SLATE_BEGIN_ARGS(SLoadingScreen)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual ~SLoadingScreen() override;

private:
	void LoadBrushes();
};
