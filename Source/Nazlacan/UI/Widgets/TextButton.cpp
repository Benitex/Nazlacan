#include "TextButton.h"
#include "Nazlacan/Macros.h"

void UTextButton::NativePreConstruct() {
    Super::NativePreConstruct();
    UpdateTextSettings(Text, TextFontSize);
}

void UTextButton::NativeConstruct() {
    Super::NativeConstruct();
    returnIfNull(Button);
    Button->OnClicked.AddDynamic(this, &UTextButton::NotifyButtonClicked);
}

// ReSharper disable once CppPassValueParameterByConstReference
void UTextButton::UpdateTextSettings(const FText NewText, const int NewFontSize) const {
    returnIfNull(TextBlock);

    TextBlock->SetText(NewText);

    FSlateFontInfo FontInfo = TextBlock->GetFont();
    FontInfo.Size = NewFontSize;
    TextBlock->SetFont(FontInfo);
}
