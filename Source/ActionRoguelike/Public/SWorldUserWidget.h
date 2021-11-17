#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "SWorldUserWidget.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget {
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, Category="UI", meta = (ExposeOnSpawn=true))
  AActor* AttachedActor;

  UPROPERTY(EditAnywhere, Category="UI")
  FVector WorldOffset;

protected:
  UPROPERTY(meta = (BindWidget))
  USizeBox* ParentSizeBox;

  virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
