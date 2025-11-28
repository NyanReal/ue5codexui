#include "FullScreenSelectableListWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"

/////////////////////////////////////////////////////
// USelectableListItemWidget

USelectableListItemWidget::USelectableListItemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = false;
}

void USelectableListItemWidget::SetupItem(int32 InIndex, const FText& InLabel, UTexture2D* InTexture)
{
	ItemIndex = InIndex;
	Label = InLabel;
	IconTexture = InTexture;

	if (LabelText)
	{
		LabelText->SetText(Label);
	}
	if (IconImage && IconTexture)
	{
		IconImage->SetBrushFromTexture(IconTexture, true);
	}
}

void USelectableListItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

TSharedRef<SWidget> USelectableListItemWidget::RebuildWidget()
{
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"));
	}

	RootBox = nullptr;
	IconImage = nullptr;
	LabelText = nullptr;

	if (WidgetTree)
	{
		WidgetTree->RootWidget = nullptr;
		RootBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("RootBox"));

		if (RootBox)
		{
			WidgetTree->RootWidget = RootBox;

			IconImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("IconImage"));
			if (IconImage)
			{
				if (IconTexture)
				{
					IconImage->SetBrushFromTexture(IconTexture, true);
				}

				if (UHorizontalBoxSlot* HSlot = RootBox->AddChildToHorizontalBox(IconImage))
				{
					HSlot->SetPadding(FMargin(4.f, 4.f, 8.f, 4.f));
					HSlot->SetVerticalAlignment(VAlign_Center);
				}
			}

			LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("LabelText"));
			if (LabelText)
			{
				LabelText->SetText(Label);

				if (UHorizontalBoxSlot* HSlot = RootBox->AddChildToHorizontalBox(LabelText))
				{
					HSlot->SetPadding(FMargin(0.f, 4.f, 4.f, 4.f));
					HSlot->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	return Super::RebuildWidget();
}

FReply USelectableListItemWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (OnItemClicked.IsBound())
	{
		OnItemClicked.Execute(ItemIndex);
	}

	return FReply::Handled();
}

/////////////////////////////////////////////////////
// UFullScreenSelectableListWidget

UFullScreenSelectableListWidget::UFullScreenSelectableListWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;

	ItemLabels =
	{
		FText::FromString(TEXT("아이템 0")),
		FText::FromString(TEXT("아이템 1")),
		FText::FromString(TEXT("아이템 2")),
		FText::FromString(TEXT("아이템 3")),
		FText::FromString(TEXT("아이템 4")),
	};

	ItemIconNames =
	{
		TEXT("Red"),
		TEXT("Blue"),
		TEXT("Green"),
		TEXT("RedBlue"),
		TEXT("BlueGreen"),
	};
}

void UFullScreenSelectableListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Visible);
}

TSharedRef<SWidget> UFullScreenSelectableListWidget::RebuildWidget()
{
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"));
	}

	RootCanvas = nullptr;
	MainBox = nullptr;
	SelectedIndexText = nullptr;
	ListScrollBox = nullptr;
	ButtonBox = nullptr;
	ConfirmButton = nullptr;
	ConfirmButtonLabel = nullptr;
	CloseButton = nullptr;
	CloseButtonLabel = nullptr;

	if (WidgetTree)
	{
		WidgetTree->RootWidget = nullptr;
		RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));

		if (RootCanvas)
		{
			WidgetTree->RootWidget = RootCanvas;

			MainBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MainBox"));
			if (MainBox)
			{
				if (UCanvasPanelSlot* CanvasSlot = RootCanvas->AddChildToCanvas(MainBox))
				{
					CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
					CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
					CanvasSlot->SetPosition(FVector2D(0.f, 0.f));
					CanvasSlot->SetSize(FVector2D(600.f, 400.f));
				}

				SelectedIndexText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SelectedIndexText"));
				if (SelectedIndexText)
				{
					if (UVerticalBoxSlot* VBoxSlot = MainBox->AddChildToVerticalBox(SelectedIndexText))
					{
						VBoxSlot->SetPadding(FMargin(4.f, 4.f, 4.f, 8.f));
						VBoxSlot->SetHorizontalAlignment(HAlign_Left);
					}
				}

				ListScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("ListScrollBox"));
				if (ListScrollBox)
				{
					if (UVerticalBoxSlot* VBoxSlot = MainBox->AddChildToVerticalBox(ListScrollBox))
					{
						VBoxSlot->SetPadding(FMargin(4.f, 0.f, 4.f, 8.f));
						VBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					}
				}

				ButtonBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ButtonBox"));
				if (ButtonBox)
				{
					if (UVerticalBoxSlot* VBoxSlot = MainBox->AddChildToVerticalBox(ButtonBox))
					{
						VBoxSlot->SetPadding(FMargin(4.f, 8.f, 4.f, 4.f));
						VBoxSlot->SetHorizontalAlignment(HAlign_Right);
						VBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
					}
				}
			}
		}
	}

	if (ButtonBox)
	{
		ConfirmButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ConfirmButton"));
		if (ConfirmButton)
		{
			ConfirmButtonLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ConfirmButtonLabel"));
			if (ConfirmButtonLabel)
			{
				ConfirmButtonLabel->SetText(FText::FromString(TEXT("확인")));
				ConfirmButton->AddChild(ConfirmButtonLabel);
			}

			if (UHorizontalBoxSlot* HSlot = ButtonBox->AddChildToHorizontalBox(ConfirmButton))
			{
				HSlot->SetPadding(FMargin(4.f, 0.f));
				HSlot->SetVerticalAlignment(VAlign_Center);
			}

			ConfirmButton->OnClicked.AddDynamic(this, &UFullScreenSelectableListWidget::HandleConfirmClicked);
		}
	}

	if (ButtonBox)
	{
		CloseButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CloseButton"));
		if (CloseButton)
		{
			CloseButtonLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CloseButtonLabel"));
			if (CloseButtonLabel)
			{
				CloseButtonLabel->SetText(FText::FromString(TEXT("닫기")));
				CloseButton->AddChild(CloseButtonLabel);
			}

			if (UHorizontalBoxSlot* HSlot = ButtonBox->AddChildToHorizontalBox(CloseButton))
			{
				HSlot->SetPadding(FMargin(4.f, 0.f));
				HSlot->SetVerticalAlignment(VAlign_Center);
			}

			CloseButton->OnClicked.AddDynamic(this, &UFullScreenSelectableListWidget::HandleCloseClicked);
		}
	}

	if (ListScrollBox)
	{
		BuildListItems();
	}

	UpdateSelectedIndexText();

	return Super::RebuildWidget();
}

void UFullScreenSelectableListWidget::BuildListItems()
{
	if (!ListScrollBox || !GetOwningPlayer())
	{
		return;
	}

	const int32 NumItems = ItemLabels.Num();
	for (int32 i = 0; i < NumItems; ++i)
	{
		const FText& Label = ItemLabels[i];

		FString IconName;
		if (ItemIconNames.IsValidIndex(i))
		{
			IconName = ItemIconNames[i];
		}

		UTexture2D* IconTex = nullptr;
		if (!IconName.IsEmpty())
		{
			IconTex = LoadIconTextureByName(IconName);
		}

		USelectableListItemWidget* ItemWidget =
			CreateWidget<USelectableListItemWidget>(GetOwningPlayer(), USelectableListItemWidget::StaticClass());

		if (!ItemWidget)
		{
			continue;
		}

		ItemWidget->SetupItem(i, Label, IconTex);

		ItemWidget->OnItemClicked.BindUObject(this, &UFullScreenSelectableListWidget::HandleItemClicked);

		ListScrollBox->AddChild(ItemWidget);
	}
}

void UFullScreenSelectableListWidget::HandleItemClicked(int32 Index)
{
	SelectedIndex = Index;
	UpdateSelectedIndexText();
}

void UFullScreenSelectableListWidget::UpdateSelectedIndexText()
{
	if (!SelectedIndexText)
	{
		return;
	}

	if (SelectedIndex == INDEX_NONE)
	{
		SelectedIndexText->SetText(FText::FromString(TEXT("선택 인덱스: 없음")));
	}
	else
	{
		SelectedIndexText->SetText(
			FText::FromString(FString::Printf(TEXT("선택 인덱스: %d"), SelectedIndex))
		);
	}
}

void UFullScreenSelectableListWidget::HandleConfirmClicked()
{
	if (GEngine)
	{
		if (SelectedIndex == INDEX_NONE)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Yellow,
				TEXT("No item selected"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Green,
				*FString::Printf(TEXT("Selected Index: %d"), SelectedIndex));
		}
	}
}

void UFullScreenSelectableListWidget::HandleCloseClicked()
{
	RemoveFromParent();
}

UTexture2D* UFullScreenSelectableListWidget::LoadIconTextureByName(const FString& IconName) const
{
	const FString AssetPath = BuildIconPath(IconName);
	return LoadObject<UTexture2D>(nullptr, *AssetPath);
}

FString UFullScreenSelectableListWidget::BuildIconPath(const FString& IconName) const
{
	return FString::Printf(TEXT("/Game/UI/Icons/%s.%s"), *IconName, *IconName);
}
