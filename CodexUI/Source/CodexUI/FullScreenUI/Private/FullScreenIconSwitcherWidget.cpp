#include "FullScreenIconSwitcherWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Texture2D.h"

UFullScreenIconSwitcherWidget::UFullScreenIconSwitcherWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;

	IconNames = {
		TEXT("Red"),
		TEXT("Blue"),
		TEXT("Green"),
	};
}

void UFullScreenIconSwitcherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Visible);
}

TSharedRef<SWidget> UFullScreenIconSwitcherWidget::RebuildWidget()
{
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"));
	}

	RootCanvas = nullptr;
	CenterBox = nullptr;
	TitleText = nullptr;
	ImageButton = nullptr;
	IconImage = nullptr;

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

				TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
				if (TitleText)
				{
					TitleText->SetText(FText::FromString(TEXT("Icon Preview")));
					TitleText->SetJustification(ETextJustify::Center);

					if (UVerticalBoxSlot* VBoxSlot = CenterBox->AddChildToVerticalBox(TitleText))
					{
						VBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));
						VBoxSlot->SetHorizontalAlignment(HAlign_Center);
					}
				}

				ImageButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ImageButton"));
				if (ImageButton)
				{
					ImageButton->OnClicked.AddDynamic(this, &UFullScreenIconSwitcherWidget::HandleImageButtonClicked);

					IconImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("IconImage"));
					if (IconImage)
					{
						ImageButton->AddChild(IconImage);
					}

					if (UVerticalBoxSlot* VBoxSlot = CenterBox->AddChildToVerticalBox(ImageButton))
					{
						VBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
						VBoxSlot->SetHorizontalAlignment(HAlign_Center);
					}
				}
			}
		}
	}

	UpdateIconTexture();

	return Super::RebuildWidget();
}

FReply UFullScreenIconSwitcherWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UFullScreenIconSwitcherWidget::HandleImageButtonClicked()
{
	if (IconNames.Num() == 0)
	{
		return;
	}

	CurrentIconIndex = (CurrentIconIndex + 1) % IconNames.Num();
	UpdateIconTexture();
}

void UFullScreenIconSwitcherWidget::UpdateIconTexture()
{
	if (!IconImage || IconNames.Num() == 0)
	{
		return;
	}

	CurrentIconIndex = FMath::Clamp(CurrentIconIndex, 0, IconNames.Num() - 1);
	const FString& IconName = IconNames[CurrentIconIndex];

	const FString AssetPath = BuildTexturePath(IconName);

	if (UTexture2D* LoadedTex = LoadObject<UTexture2D>(nullptr, *AssetPath))
	{
		IconImage->SetBrushFromTexture(LoadedTex, true);
	}
	else
	{
		if (TitleText)
		{
			TitleText->SetText(FText::FromString(FString::Printf(TEXT("Failed: %s"), *AssetPath)));
		}
	}
}

FString UFullScreenIconSwitcherWidget::BuildTexturePath(const FString& IconName) const
{
	return FString::Printf(TEXT("/Game/UI/Icons/%s.%s"), *IconName, *IconName);
}
