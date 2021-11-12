#include "SActionComponent.h"

#include "SAction.h"

USActionComponent::USActionComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
  if (!ensure(ActionClass)) {
    return;
  }

  USAction* NewAction = NewObject<USAction>(this, ActionClass);

  if (ensure(NewAction)) {
    Actions.Add(NewAction);
    if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {
      NewAction->StartAction(Instigator);
    }
  }
}

USAction* USActionComponent::GetAction(const TSubclassOf<USAction> ActionToGet) {
  for (USAction* Action : Actions) {
    if (Action->IsA(ActionToGet)) {
      return Action;
    }
  }
  return nullptr;
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
