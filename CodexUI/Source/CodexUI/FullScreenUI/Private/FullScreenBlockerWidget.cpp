#include "FullScreenBlockerWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

UFullScreenBlockerWidget::UFullScreenBlockerWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UFullScreenBlockerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!WidgetTree)
	{
		return;
	}

	WidgetTree->RootWidget = nullptr;
	RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootCanvas;

	CenterText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CenterText"));
	if (CenterText)
	{
		CenterText->SetText(FText::FromString(TEXT("FullScreen Blocker")));
		CenterText->SetJustification(ETextJustify::Center);

		if (UCanvasPanelSlot* CanvasSlot = RootCanvas->AddChildToCanvas(CenterText))
		{
			CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetPosition(FVector2D(0.f, 0.f));
			CanvasSlot->SetSize(FVector2D(0.f, 0.f));
		}
	}

	SetVisibility(ESlateVisibility::Visible);
}

FReply UFullScreenBlockerWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}