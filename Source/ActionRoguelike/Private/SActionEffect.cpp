#include "SActionEffect.h"

#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"

USActionEffect::USActionEffect() {
  bAutoStart = true;
}

float USActionEffect::GetTimeRemaining() const {
  AGameStateBase* GS = GetWorld()->GetGameState();

  if (GS) {
    const float EndTime = TimeStarted + Duration;

    // Only the server really knows the actual running time,
    // so ask the server what the current time is
    return EndTime - GS->GetServerWorldTimeSeconds();
  }

  return Duration;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator) {
  Super::StartAction_Implementation(Instigator);

  if (Duration > 0.f) {
    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "StopAction", Instigator);
    GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
  }

  if (Period > 0.0f) {
    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
    GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
  }
}

void USActionEffect::StopAction_Implementation(AActor* Instigator) {
  if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) {
    ExecutePeriodicEffect(Instigator);
  }

  Super::StopAction_Implementation(Instigator);

  GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
  GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

  USActionComponent* Comp = GetOwningComponent();
  if (Comp) {
    Comp->RemoveAction(this);
  }
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator) { }
