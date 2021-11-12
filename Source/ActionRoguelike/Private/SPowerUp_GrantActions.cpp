#include "SPowerUp_GrantActions.h"

#include "SAction.h"
#include "SActionComponent.h"

void ASPowerUp_GrantActions::Interact_Implementation(APawn* InstigatorPawn) {
  Super::Interact_Implementation(InstigatorPawn);

  USActionComponent* ActionComp = Cast<USActionComponent>(
    InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass())
  );

  if (ActionComp) {
    bool bGrantedAction = false;
    for (const TSubclassOf<USAction> Action : ActionClasses) {
      if (ActionComp->GetAction(Action)) {
        FString DebugMsg = FString::Printf(TEXT("Action '%s' already known"), *GetNameSafe(Action));
        GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, DebugMsg);
        continue;
      }
      ActionComp->AddAction(InstigatorPawn, Action);
      bGrantedAction = true;
    }
    if (bGrantedAction) {
      Deactivate();
    }
  }
}
