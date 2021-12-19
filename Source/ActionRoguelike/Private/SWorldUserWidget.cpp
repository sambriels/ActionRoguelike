#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime) {
  Super::NativeTick(MyGeometry, InDeltaTime);

  if (!IsValid(AttachedActor)) {
    RemoveFromParent();
    UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget"));
    return;
  }

  FVector2D ScreenPosition;
  const bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(
    GetOwningPlayer(),
    AttachedActor->GetActorLocation() + WorldOffset,
    ScreenPosition
  );
  if (bIsOnScreen) {
    const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

    ScreenPosition /= Scale;

    if (ParentSizeBox) {
      ParentSizeBox->SetRenderTranslation(ScreenPosition);
    }
  }

  if (ParentSizeBox) {
    ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
  }
}
