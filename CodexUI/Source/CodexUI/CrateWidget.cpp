#include "CrateWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

void UCrateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EnsureCanvasRoot();
	BuildPlaceholderLabel();
}

void UCrateWidget::EnsureCanvasRoot()
{
	if (!WidgetTree)
	{
		return;
	}

	if (WidgetTree->RootWidget)
	{
		if (WidgetTree->RootWidget->IsA<UCanvasPanel>())
		{
			return;
		}
	}

	if (UCanvasPanel* CanvasRoot = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("AutoCanvasRoot")))
	{
		WidgetTree->RootWidget = CanvasRoot;
	}
}

void UCrateWidget::BuildPlaceholderLabel()
{
	if (!WidgetTree)
	{
		return;
	}

	UCanvasPanel* CanvasRoot = Cast<UCanvasPanel>(WidgetTree->RootWidget);
	if (!CanvasRoot)
	{
		return;
	}

	const FName LabelWidgetName(TEXT("KivokovCrateCenterLabel"));
	const int32 ChildCount = CanvasRoot->GetChildrenCount();
	for (int32 Index = 0; Index < ChildCount; ++Index)
	{
		if (CanvasRoot->GetChildAt(Index)->GetFName() == LabelWidgetName)
		{
			return;
		}
	}

	if (UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), LabelWidgetName))
	{
		Label->SetText(FText::FromString(TEXT("KivokovCrate")));
		Label->SetJustification(ETextJustify::Center);

		if (UCanvasPanelSlot* CanvasSlot = CanvasRoot->AddChildToCanvas(Label))
		{
			CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetAutoSize(true);
		}
	}
}
