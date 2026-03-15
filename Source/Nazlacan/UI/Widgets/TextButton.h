#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TextButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

UCLASS()
class NAZLACAN_API UTextButton : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TextFontSize = DefaultTextFontSize;

	static constexpr int DefaultTextFontSize = 32;

public:
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnButtonClicked OnButtonClicked;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	void UpdateTextSettings(const FText NewText, const int NewFontSize = DefaultTextFontSize) const;

	UFUNCTION()
	virtual void NotifyButtonClicked() { OnButtonClicked.Broadcast(); }
};
