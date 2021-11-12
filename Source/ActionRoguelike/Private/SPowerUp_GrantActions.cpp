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
    for (TSubclassOf<USAction> Action : ActionClasses) {
      if (ActionComp->AddAction(InstigatorPawn, *Action)) {
        bGrantedAction = true;
      }
    }
    if (bGrantedAction) {
      Deactivate();
    }
  }
}
