#include "SActionComponent.h"

#include "SAction.h"

USActionComponent::USActionComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

bool USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
  if (!ensure(ActionClass)) {
    return false;
  }

  USAction* NewAction = NewObject<USAction>(this, ActionClass);

  if (ensure(NewAction)) {
    if (HasAction(NewAction->ActionName)) {
      UE_LOG(
        LogTemp,
        Warning,
        TEXT("Already has action: %s, not adding another one"),
        *NewAction->ActionName.ToString()
      );
      return false;
    }
    Actions.Add(NewAction);
    UE_LOG(LogTemp, Warning, TEXT("Added new action: %s"), *NewAction->ActionName.ToString());
    if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {
      NewAction->StartAction(Instigator);
    }
    return true;
  }
  return false;
}

bool USActionComponent::HasAction(const FName ActionName) {
  for (USAction* Action : Actions) {
    if (Action && Action->ActionName == ActionName) {
      return true;
    }
  }
  return false;
}

void USActionComponent::RemoveAction(USAction* ActionToRemove) {
  if (ActionToRemove && ensure(!ActionToRemove->IsRunning())) {
    return;
  }
  Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName) {
  for (USAction* Action : Actions) {
    if (Action && Action->ActionName == ActionName) {
      if (!Action->CanStart(Instigator)) {
        FString FailedMessage = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMessage);
        continue;;
      }
      Action->StartAction(Instigator);
      return true;
    }
  }
  return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName) {
  for (USAction* Action : Actions) {
    if (Action && Action->ActionName == ActionName) {
      if (Action->IsRunning()) {
        Action->StopAction(Instigator);
        return true;
      }
    }
  }
  return false;
}

void USActionComponent::BeginPlay() {
  Super::BeginPlay();

  for (const TSubclassOf<USAction> ActionClass : DefaultActions) {
    AddAction(GetOwner(), ActionClass);
  }
}

void USActionComponent::TickComponent(
  float DeltaTime,
  ELevelTick TickType,
  FActorComponentTickFunction* ThisTickFunction
) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  const FString DebugMessage = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
  GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMessage);
}
