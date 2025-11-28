#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullScreenCloseWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;
class UVerticalBox;

/**
 * 화면 전체를 덮고, 닫기 버튼이 있는 위젯
 * - 루트: CanvasPanel
 * - 중앙: 텍스트 + 닫기 버튼
 */
UCLASS()
class CODEXUI_API UFullScreenCloseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFullScreenCloseWidget(const FObjectInitializer& ObjectInitializer);

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
	TObjectPtr<UTextBlock> CenterText;

	UPROPERTY()
	TObjectPtr<UButton> CloseButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> CloseButtonLabel;

	UFUNCTION()
	void HandleCloseButtonClicked();
};
