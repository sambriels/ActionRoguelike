#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USInteractionComponent();

  void PrimaryInteract();

protected:
  UPROPERTY()
  AActor* FocusedActor;

  UPROPERTY(EditDefaultsOnly, Category="UI")
  TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

  UPROPERTY(EditDefaultsOnly, Category="Trace")
  float TraceDistance;

  UPROPERTY(EditDefaultsOnly, Category="Trace")
  float TraceRadius;

  UPROPERTY(EditDefaultsOnly, Category="Trace")
  TEnumAsByte<ECollisionChannel> CollisionChannel;

  UPROPERTY()
  USWorldUserWidget* DefaultWidgetInstance;

  void FindBestInteractable();

  virtual void TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
  ) override;
};
