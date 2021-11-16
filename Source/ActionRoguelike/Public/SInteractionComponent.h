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
  // Reliable - Will always arrive, eventually. Request will be re-sent unless an acknoledgment was received
  // Unreliable - Not guaranteed, packet can get lost and won't retry
  UFUNCTION(Server, Reliable)
  void ServerInteract(AActor* InFocus);

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
