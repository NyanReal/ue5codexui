#include "FullScreenMessageWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

UFullScreenMessageWidget::UFullScreenMessageWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;

	Messages.Add(FText::FromString(TEXT("메시지 1: Hello")));
	Messages.Add(FText::FromString(TEXT("메시지 2: Welcome")));
	Messages.Add(FText::FromString(TEXT("메시지 3: FullScreen UI")));
}

void UFullScreenMessageWidget::NativeConstruct()
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
		ChangeMessageButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ChangeMessageButton"));
		if (ChangeMessageButton)
		{
			ChangeMessageLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ChangeMessageLabel"));
			if (ChangeMessageLabel)
			{
				ChangeMessageLabel->SetText(FText::FromString(TEXT("메시지 바꾸기")));
				ChangeMessageLabel->SetJustification(ETextJustify::Center);
				ChangeMessageButton->AddChild(ChangeMessageLabel);
			}

			if (UVerticalBoxSlot* VBoxSlot = CenterBox->AddChildToVerticalBox(ChangeMessageButton))
			{
				VBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 8.f));
				VBoxSlot->SetHorizontalAlignment(HAlign_Center);
			}

			ChangeMessageButton->OnClicked.AddDynamic(this, &UFullScreenMessageWidget::HandleChangeMessageClicked);
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

			CloseButton->OnClicked.AddDynamic(this, &UFullScreenMessageWidget::HandleCloseButtonClicked);
		}
	}

	UpdateCenterText();

	SetVisibility(ESlateVisibility::Visible);
}

FReply UFullScreenMessageWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UFullScreenMessageWidget::HandleChangeMessageClicked()
{
	if (Messages.Num() == 0)
	{
		return;
	}

	CurrentMessageIndex = (CurrentMessageIndex + 1) % Messages.Num();
	UpdateCenterText();
}

void UFullScreenMessageWidget::HandleCloseButtonClicked()
{
	RemoveFromParent();
}

void UFullScreenMessageWidget::UpdateCenterText()
{
	if (!CenterText)
	{
		return;
	}

	if (Messages.Num() == 0)
	{
		CenterText->SetText(FText::FromString(TEXT("No Messages")));
	}
	else
	{
		CurrentMessageIndex = FMath::Clamp(CurrentMessageIndex, 0, Messages.Num() - 1);
		CenterText->SetText(Messages[CurrentMessageIndex]);
	}
}