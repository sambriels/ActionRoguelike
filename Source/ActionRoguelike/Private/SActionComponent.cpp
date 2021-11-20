#include "SActionComponent.h"

#include "SAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

USActionComponent::USActionComponent() {
  PrimaryComponentTick.bCanEverTick = true;

  SetIsReplicatedByDefault(true);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
  if (!ensure(ActionClass)) {
    return;
  }

  // Skip for clients
  if (!GetOwner()->HasAuthority()) {
    UE_LOG(
      LogTemp,
      Warning,
      TEXT("Client attempting to AddAction. [Class %s]"),
      *GetNameSafe(ActionClass)
    );
  }

  USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);

  if (ensure(NewAction)) {
    NewAction->Initialize(this);
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

      // Is client?
      if (!GetOwner()->HasAuthority()) {
        ServerStartAction(Instigator, ActionName);
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
        // Is client?
        if (!GetOwner()->HasAuthority()) {
          ServerStopAction(Instigator, ActionName);
        }

        Action->StopAction(Instigator);
        return true;
      }
    }
  }
  return false;
}

void USActionComponent::BeginPlay() {
  Super::BeginPlay();

  // Server only
  if (GetOwner()->HasAuthority()) {
    for (const TSubclassOf<USAction> ActionClass : DefaultActions) {
      AddAction(GetOwner(), ActionClass);
    }
  }
}

void USActionComponent::TickComponent(
  float DeltaTime,
  ELevelTick TickType,
  FActorComponentTickFunction* ThisTickFunction
) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // const FString DebugMessage = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
  // GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMessage);

  for (const USAction* Action : Actions) {
    const FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

    const FString ActionMsg = FString::Printf(
      TEXT("[%s] Action: %s"),
      *GetNameSafe(GetOwner()),
      *GetNameSafe(Action)
    );

    LogOnScreen(this, ActionMsg, TextColor, 0.f);
  }
}

bool USActionComponent::ReplicateSubobjects(
  UActorChannel* Channel,
  FOutBunch* Bunch,
  FReplicationFlags* RepFlags
) {
  bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

  for (USAction* Action : Actions) {
    if (Action) {
      WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
    }
  }
  return WroteSomething;
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName) {
  StopActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName) {
  StartActionByName(Instigator, ActionName);
}

void USActionComponent::GetLifetimeReplicatedProps(
  TArray<FLifetimeProperty>& OutLifetimeProps
) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(USActionComponent, Actions);
}
