#include "SAction.h"

#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp) {
  ActionComp = NewActionComp;
}

void USAction::StartAction_Implementation(AActor* Instigator) {
  // UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
  LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
  USActionComponent* Comp = GetOwningComponent();
  Comp->ActiveGameplayTags.AppendTags(GrantsTags);

  bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator) {
  // UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
  LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

  // ensureAlways(bIsRunning);

  USActionComponent* Comp = GetOwningComponent();
  Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

  bIsRunning = false;
}

bool USAction::CanStart_Implementation(AActor* Instigator) {
  /* Action is already running */
  if (IsRunning()) {
    return false;
  }

  const USActionComponent* Comp = GetOwningComponent();

  if (Comp->ActiveGameplayTags.HasAny(BlockedTags)) {
    return false;
  }
  return true;
}

UWorld* USAction::GetWorld() const {
  // Outer has been set when creating action via NewObject<T> in ActionComponent.cpp
  // `AddAction` function
  AActor* Actor = Cast<AActor>(GetOuter());

  if (Actor) {
    return Actor->GetWorld();
  }
  return nullptr;
}

bool USAction::IsRunning() const {
  return bIsRunning;
}

void USAction::OnRep_IsRunning() {
  if (bIsRunning) {
    StartAction(nullptr);
  } else {
    StopAction(nullptr);
  }
}

USActionComponent* USAction::GetOwningComponent() const {
  return ActionComp;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(USAction, bIsRunning);
  DOREPLIFETIME(USAction, ActionComp);
}
