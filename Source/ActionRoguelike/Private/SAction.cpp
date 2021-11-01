#include "SAction.h"

void USAction::StartAction_Implementation(AActor* Instigator) {
  UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* Instigator) {
  UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const {
  // Outer has been set when creating action via NewObject<T> in ActionComponent.cpp
  // `AddAction` function
  UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());

  if (ActorComp) {
    return ActorComp->GetWorld();
  }
  return nullptr;
}
