#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FullScreenSelectableListWidget.generated.h"

class UCanvasPanel;
class UScrollBox;
class UHorizontalBox;
class UVerticalBox;
class UImage;
class UTextBlock;
class UButton;
class UTexture2D;

// 리스트 아이템 클릭 델리게이트
DECLARE_DELEGATE_OneParam(FOnListItemClicked, int32 /*Index*/);

/**
 * 스크롤 리스트의 개별 아이템 (이미지 + 텍스트)
 */
UCLASS()
class CODEXUI_API USelectableListItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USelectableListItemWidget(const FObjectInitializer& ObjectInitializer);

	void SetupItem(int32 InIndex, const FText& InLabel, UTexture2D* InTexture);

	FOnListItemClicked OnItemClicked;

	int32 GetItemIndex() const { return ItemIndex; }

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY()
	TObjectPtr<UHorizontalBox> RootBox;

	UPROPERTY()
	TObjectPtr<UImage> IconImage;

	UPROPERTY()
	TObjectPtr<UTextBlock> LabelText;

	int32 ItemIndex = INDEX_NONE;
	FText Label;

	UPROPERTY()
	TObjectPtr<UTexture2D> IconTexture;
};

/**
 * 풀스크린 선택 리스트 위젯
 * - 스크롤 가능한 이미지+텍스트 리스트
 * - 선택 인덱스 표시 텍스트
 * - 확인 버튼(선택 인덱스를 디버그 메시지로 출력)
 * - 닫기 버튼
 */
UCLASS()
class CODEXUI_API UFullScreenSelectableListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFullScreenSelectableListWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY()
	TObjectPtr<UVerticalBox> MainBox;

	UPROPERTY()
	TObjectPtr<UTextBlock> SelectedIndexText;

	UPROPERTY()
	TObjectPtr<UScrollBox> ListScrollBox;

	UPROPERTY()
	TObjectPtr<UHorizontalBox> ButtonBox;

	UPROPERTY()
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> ConfirmButtonLabel;

	UPROPERTY()
	TObjectPtr<UButton> CloseButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> CloseButtonLabel;

	UPROPERTY(EditAnywhere, Category = "List")
	TArray<FText> ItemLabels;

	UPROPERTY(EditAnywhere, Category = "List")
	TArray<FString> ItemIconNames;

	int32 SelectedIndex = INDEX_NONE;

	void BuildListItems();
	void UpdateSelectedIndexText();

	UTexture2D* LoadIconTextureByName(const FString& IconName) const;
	FString BuildIconPath(const FString& IconName) const;

	void HandleItemClicked(int32 Index);

	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCloseClicked();
};
