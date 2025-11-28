#include "FullScreenCloseWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

UFullScreenCloseWidget::UFullScreenCloseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UFullScreenCloseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!WidgetTree)
	{
		return;
	}

	WidgetTree->RootWidget = nullptr;
	RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootCanvas;

	CenterBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CenterBox"));
	if (CenterBox)
	{
		if (UCanvasPanelSlot* CanvasSlot = RootCanvas->AddChildToCanvas(CenterBox))
		{
			CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetPosition(FVector2D(0.f, 0.f));
			CanvasSlot->SetSize(FVector2D(0.f, 0.f));
		}
	}

	if (CenterBox)
	{
		CenterText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CenterText"));
		if (CenterText)
		{
			CenterText->SetText(FText::FromString(TEXT("FullScreen Overlay")));
			CenterText->SetJustification(ETextJustify::Center);

			if (UVerticalBoxSlot* VBoxSlot = CenterBox->AddChildToVerticalBox(CenterText))
			{
				VBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
				VBoxSlot->SetHorizontalAlignment(HAlign_Center);
			}
		}
	}

	if (CenterBox)
	{
		CloseButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CloseButton"));
		if (CloseButton)
		{
			CloseButtonLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CloseButtonLabel"));
			if (CloseButtonLabel)
			{
				CloseButtonLabel->SetText(FText::FromString(TEXT("닫기")));
				CloseButtonLabel->SetJustification(ETextJustify::Center);
				CloseButton->AddChild(CloseButtonLabel);
			}

			if (UVerticalBoxSlot* VBoxSlot = CenterBox->AddChildToVerticalBox(CloseButton))
			{
				VBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
				VBoxSlot->SetHorizontalAlignment(HAlign_Center);
			}

			CloseButton->OnClicked.AddDynamic(this, &UFullScreenCloseWidget::HandleCloseButtonClicked);
		}
	}

	SetVisibility(ESlateVisibility::Visible);
}

FReply UFullScreenCloseWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UFullScreenCloseWidget::HandleCloseButtonClicked()
{
	RemoveFromParent();
}