# FullScreenUI 위젯 모음

언리얼에서 **화면을 가득 채우고 마우스 입력을 가로채는 UMG 위젯들**을 전부 C++ 코드만으로 구성한 샘플입니다.  
이 프로젝트 안의 모든 위젯은 **블루프린트 에셋 없이 클래스만으로 생성해서 바로 화면에 붙일 수 있도록** 작성되어 있습니다.

> 모듈/프로젝트 이름은 예시로 `FullScreenUI`를 사용했고,  
> 클래스의 API 매크로는 `YOURPROJECT_API` 로 되어 있으니  
> 실제 프로젝트 모듈 이름에 맞게 `XXX_API` 로 바꿔 사용하면 됩니다.

---

## 1. 공통 요구사항 정리 (요청한 내용)

1. **화면 전체를 덮는 UMG 위젯**
   - `AddToViewport()`만 호출하면 뷰포트를 가득 채우는 오버레이로 동작
   - 마우스 입력을 가로채거나(UI 전용), 혹은 리스트 스크롤을 위해 적절히 처리

2. **UI 구성은 전부 C++ 코드에서만 생성**
   - `WidgetTree`를 사용해서 루트와 자식 위젯을 `NativeConstruct()` 안에서 전부 생성
   - 별도의 UMG Widget Blueprint 에셋 없이도 동작해야 함

3. **여러 가지 변형 위젯**
   - 단순 텍스트만 있는 풀스크린 블로커
   - 닫기 버튼이 있는 버전
   - 버튼을 눌러 텍스트가 배열 내 문자열로 바뀌는 버전
   - 이미지를 눌렀을 때 `UI/Icons` 경로 아래 텍스처들을 순환 로딩해서 교체하는 버전
   - 스크롤 가능한 **이미지+텍스트 리스트**와
     - 선택된 인덱스를 표시하는 텍스트
     - 선택 인덱스로 디버그 메시지를 출력하는 **확인 버튼**
     - 위젯을 닫는 **닫기 버튼**

---

## 2. 구현된 클래스 목록 (구현 내용 요약)

### 2.1 `UFullScreenBlockerWidget`

- 파일
  - `Source/FullScreenUI/Public/FullScreenBlockerWidget.h`
  - `Source/FullScreenUI/Private/FullScreenBlockerWidget.cpp`
- 역할
  - 루트: `UCanvasPanel`
  - 중앙에 텍스트 1개 (`UTextBlock`)
  - `NativeOnMouseButtonDown()`에서 항상 `FReply::Handled()`를 반환해
    뷰포트 아래로 마우스 클릭이 전달되지 않도록 함
- 특징
  - 가장 기본적인 **풀스크린 입력 블로커** 역할을 하는 위젯

---

### 2.2 `UFullScreenCloseWidget`

- 파일
  - `Source/FullScreenUI/Public/FullScreenCloseWidget.h`
  - `Source/FullScreenUI/Private/FullScreenCloseWidget.cpp`
- 역할
  - 루트: `UCanvasPanel`
  - 중앙: `UVerticalBox`
    - 안내 텍스트
    - “닫기” 버튼
  - “닫기” 버튼 클릭 시 `RemoveFromParent()` 호출로 자기 자신을 닫음
- 특징
  - 풀스크린 오버레이 + **닫기 버튼** 조합의 기본형

---

### 2.3 `UFullScreenMessageWidget`

- 파일
  - `Source/FullScreenUI/Public/FullScreenMessageWidget.h`
  - `Source/FullScreenUI/Private/FullScreenMessageWidget.cpp`
- 역할
  - 루트: `UCanvasPanel`
  - 중앙: `UVerticalBox`
    - 텍스트 표시 영역
    - “메시지 바꾸기” 버튼
    - “닫기” 버튼
  - 내부에 `TArray<FText> Messages` 를 가지고 있으며,
    “메시지 바꾸기” 버튼을 누를 때마다 인덱스를 하나씩 증가시키며 순환(cyclic)하면서 텍스트를 변경
- 특징
  - 버튼 입력으로 **정해진 배열 내 문자열을 순환 표시**하는 샘플
  - 메시지 배열은 C++/에디터 `EditAnywhere` 에서 설정 가능

---

### 2.4 `UFullScreenIconSwitcherWidget`

- 파일
  - `Source/FullScreenUI/Public/FullScreenIconSwitcherWidget.h`
  - `Source/FullScreenUI/Private/FullScreenIconSwitcherWidget.cpp`
- 역할
  - 루트: `UCanvasPanel`
  - 중앙: `UVerticalBox`
    - 타이틀 텍스트
    - 이미지 버튼 (`UButton` + `UImage`)
  - 내부에 `TArray<FString> IconNames` 를 가지고 있으며 기본값은 `{"Red", "Blue", "Green"}` 등
  - 이미지(버튼)를 클릭할 때마다 현재 인덱스를 증가시키며 `IconNames` 배열을 순환하고,
    각 이름에 대해 `/Game/UI/Icons/{Name}.{Name}` 경로의 `UTexture2D` 를 `LoadObject`로 로드하여 이미지에 적용
- 특징
  - 정해진 문자열 배열(아이콘 이름)과 `/Game/UI/Icons` 경로를 이용해
    **아이콘 텍스처를 런타임에 교체**하는 샘플
  - 실제 프로젝트에서는 `BuildTexturePath()` 를 수정해 파일명 규칙에 맞출 수 있음

---

### 2.5 `USelectableListItemWidget` & `UFullScreenSelectableListWidget`

- 파일
  - `Source/FullScreenUI/Public/FullScreenSelectableListWidget.h`
  - `Source/FullScreenUI/Private/FullScreenSelectableListWidget.cpp`
- 역할 (행 위젯: `USelectableListItemWidget`)
  - 각 리스트 아이템은
    - `UHorizontalBox` 루트
    - 좌측: 아이콘 이미지 (`UImage`)
    - 우측: 라벨 텍스트 (`UTextBlock`)
  - `SetupItem(int32 InIndex, const FText& InLabel, UTexture2D* InTexture)` 로 데이터 주입
  - `NativeOnMouseButtonDown()`에서 내부 델리게이트 `OnItemClicked(Index)` 호출

- 역할 (메인 위젯: `UFullScreenSelectableListWidget`)
  - 루트: `UCanvasPanel`
  - 중앙: `UVerticalBox` (크기 600 x 400 예시)
    1. 선택 인덱스 표시 텍스트 (`SelectedIndexText`)
    2. 스크롤 박스 (`UScrollBox`) 안에 `USelectableListItemWidget` 여러 개를 생성해 추가
    3. 하단 `UHorizontalBox` 에
       - “확인” 버튼
       - “닫기” 버튼
  - 선택 로직
    - 각 아이템이 클릭되면 `HandleItemClicked(int32 Index)` 가 호출돼 `SelectedIndex`를 갱신
    - `UpdateSelectedIndexText()`로 “선택 인덱스: N” 형식으로 갱신
  - 확인 버튼
    - `HandleConfirmClicked()`에서
      - 선택된 인덱스가 없으면 `No item selected`
      - 있으면 `Selected Index: N` 을 `GEngine->AddOnScreenDebugMessage` 로 출력
  - 닫기 버튼
    - `HandleCloseClicked()`에서 `RemoveFromParent()` 호출

- 특징
  - **이미지 + 텍스트가 있는 스크롤 가능한 리스트**
  - 현재 선택된 인덱스를 텍스트로 보여주고
  - 버튼으로 선택 결과를 On-Screen Debug 메시지로 확인하는 샘플

---

## 3. 공통 구현 패턴

모든 위젯은 다음과 같은 패턴을 사용합니다.

1. **루트 위젯을 코드에서 생성**
   ```cpp
   if (WidgetTree)
   {
       WidgetTree->RootWidget = nullptr;
       RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
       WidgetTree->RootWidget = RootCanvas;
   }
   ```

2. **위젯 간 배치는 슬롯 API로 제어**
   - `UCanvasPanelSlot`, `UVerticalBoxSlot`, `UHorizontalBoxSlot` 등을 사용
   - 앵커/정렬/패딩/크기 조절을 모두 코드에서 수행

3. **마우스/입력 처리**
   - 풀스크린 입력 블로커 계열 위젯은 `NativeOnMouseButtonDown()`에서 `FReply::Handled()`를 반환
   - 리스트는 행 위젯의 `NativeOnMouseButtonDown()`에서 델리게이트를 통해 상위 위젯으로 인덱스를 전달

4. **런타임 텍스처 로딩**
   - 아이콘 관련 클래스는 `LoadObject<UTexture2D>` 이용
   - `/Game/UI/Icons/{Name}.{Name}` 규칙으로 에셋을 찾도록 구현
   - 실제 에셋 이름과 경로에 맞게 `BuildIconPath()`/`BuildTexturePath()`를 수정하면 됨

---

## 4. 사용 방법 (프로젝트에 적용하기)

### 4.1 소스 파일 복사

1. 이 리포지토리(또는 ZIP)에서 `Source/FullScreenUI` 폴더를 통째로 복사
2. 자신의 프로젝트의 `Source` 폴더 아래에 붙여넣기
   - 예: `YourProject/Source/FullScreenUI/...`

3. `FullScreenUI` 모듈을 `.uproject` 또는 `.uplugin`에 추가하거나,
   이미 존재하는 게임 모듈로 옮겨서 사용해도 됩니다.
   - 이 경우, 각 헤더의 `YOURPROJECT_API` 를 실제 모듈의 API 매크로로 변경하세요.

### 4.2 빌드 설정

- `FullScreenUI.Build.cs` 를 생성해서 모듈을 등록하거나
- 기존 게임 모듈에 `.h/.cpp`만 옮겨서 사용해도 무방합니다.
- UMG/Slate 관련 모듈이 필요할 수 있으니,
  `UMG`, `Slate`, `SlateCore` 등을 `PublicDependencyModuleNames`/`PrivateDependencyModuleNames`에 포함시키세요.

### 4.3 위젯 띄우기 예시

예: `APlayerController`에서 풀스크린 선택 리스트 위젯 띄우기

```cpp
#include "FullScreenSelectableListWidget.h"

void AMyPlayerController::ShowSelectableListOverlay()
{
    if (!IsLocalController())
    {
        return;
    }

    if (UFullScreenSelectableListWidget* Widget =
        CreateWidget<UFullScreenSelectableListWidget>(this, UFullScreenSelectableListWidget::StaticClass()))
    {
        Widget->AddToViewport(100);

        // 게임 입력을 UI 전용으로 막고 싶을 때:
        // FInputModeUIOnly InputMode;
        // InputMode.SetWidgetToFocus(Widget->TakeWidget());
        // InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        // SetInputMode(InputMode);
        // bShowMouseCursor = true;
    }
}
```

다른 위젯들 (`UFullScreenBlockerWidget`, `UFullScreenCloseWidget`,  
`UFullScreenMessageWidget`, `UFullScreenIconSwitcherWidget`)도  
같은 방식으로 `CreateWidget` → `AddToViewport`로 사용하면 됩니다.

---

## 5. 커스터마이징 포인트

- **텍스트/버튼 레이블 변경**
  - 각 위젯의 `NativeConstruct()` 또는 생성자에서 `SetText()` 부분 수정
- **메시지/아이콘 배열 변경**
  - `Messages`, `IconNames`, `ItemLabels`, `ItemIconNames` 등을
    생성자 또는 에디터 `EditAnywhere`로 원하는 값으로 교체
- **레이아웃/스타일 수정**
  - Canvas/VerticalBox의 크기, 패딩, 정렬을 코드에서 변경
  - 스타일을 더 세밀하게 꾸미고 싶다면 `WidgetStyle`이나
    별도의 스타일 위젯을 추가해서 사용 가능

---