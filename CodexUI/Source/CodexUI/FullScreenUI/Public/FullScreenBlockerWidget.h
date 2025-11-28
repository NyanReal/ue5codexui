#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullScreenBlockerWidget.generated.h"

class UCanvasPanel;
class UTextBlock;

/**
 * 화면 전체를 덮는 풀스크린 블로커 위젯
 * - 루트: CanvasPanel
 * - 중앙 텍스트 1개
 * - 마우스 클릭을 모두 가로채서 밑으로 전달되지 않도록 함
 */
UCLASS()
class CODEXUI_API UFullScreenBlockerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFullScreenBlockerWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	// 마우스 입력 가로채기
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY()
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY()
	TObjectPtr<UTextBlock> CenterText;
};
