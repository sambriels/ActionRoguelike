#include "SAction.h"

#include "SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator) {
  UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

  USActionComponent* ActionComp = GetOwningComponent();
  ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

  bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator) {
  UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

  ensureAlways(bIsRunning);

  USActionComponent* ActionComp = GetOwningComponent();
  ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

  bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator) {
  /* Action is already running */
  if (IsRunning()) {
    return false;
  }

  const USActionComponent* ActionComp = GetOwningComponent();

  if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags)) {
    return false;
  }
  return true;
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

bool USAction::IsRunning() const {
  return bIsRunning;
}

USActionComponent* USAction::GetOwningComponent() const {
  return Cast<USActionComponent>(GetOuter());
}
