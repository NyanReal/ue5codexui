#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullScreenIconSwitcherWidget.generated.h"

class UCanvasPanel;
class UVerticalBox;
class UTextBlock;
class UButton;
class UImage;
class UTexture2D;

/**
 * 화면 전체를 덮고,
 * 중앙 아이콘 이미지를 클릭하면
 * UI/Icons 경로의 텍스처를 순환하면서 교체하는 위젯
 */
UCLASS()
class CODEXUI_API UFullScreenIconSwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFullScreenIconSwitcherWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY()
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY()
	TObjectPtr<UVerticalBox> CenterBox;

	UPROPERTY()
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY()
	TObjectPtr<UButton> ImageButton;

	UPROPERTY()
	TObjectPtr<UImage> IconImage;

	// 아이콘 이름 배열 (예: Red / Blue / Green / RedBlue 등)
	UPROPERTY(EditAnywhere, Category = "Icon")
	TArray<FString> IconNames;

	int32 CurrentIconIndex = 0;

	UFUNCTION()
	void HandleImageButtonClicked();

	void UpdateIconTexture();
	FString BuildTexturePath(const FString& IconName) const;
};
