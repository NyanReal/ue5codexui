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

	SetVisibility(ESlateVisibility::Visible);
}

TSharedRef<SWidget> UFullScreenCloseWidget::RebuildWidget()
{
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"));
	}

	RootCanvas = nullptr;
	CenterBox = nullptr;
	CenterText = nullptr;
	CloseButton = nullptr;
	CloseButtonLabel = nullptr;

	if (WidgetTree)
	{
		WidgetTree->RootWidget = nullptr;
		RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));

		if (RootCanvas)
		{
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
					CanvasSlot->SetAutoSize(true);
				}

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
		}
	}

	return Super::RebuildWidget();
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
