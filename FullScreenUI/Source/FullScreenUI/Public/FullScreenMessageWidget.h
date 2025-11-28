#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullScreenMessageWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;
class UVerticalBox;

/**
 * 화면 전체를 덮고,
 * - 중앙 텍스트
 * - 메시지 변경 버튼
 * - 닫기 버튼
 * 을 가진 위젯
 */
UCLASS()
class YOURPROJECT_API UFullScreenMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFullScreenMessageWidget(const FObjectInitializer& ObjectInitializer);

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
	TObjectPtr<UButton> ChangeMessageButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> ChangeMessageLabel;

	UPROPERTY()
	TObjectPtr<UButton> CloseButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> CloseButtonLabel;

	// 텍스트 교체용 메시지 배열
	UPROPERTY(EditAnywhere, Category = "Message")
	TArray<FText> Messages;

	int32 CurrentMessageIndex = 0;

	UFUNCTION()
	void HandleChangeMessageClicked();

	UFUNCTION()
	void HandleCloseButtonClicked();

	void UpdateCenterText();
};