#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrateWidget.generated.h"

class UCanvasPanel;

UCLASS()
class CODEXUI_API UCrateWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void EnsureCanvasRoot();
	void BuildPlaceholderLabel();
};
