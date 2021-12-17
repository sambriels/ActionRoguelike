#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() {
  HealthAmount = 20.f;
  CreditsCost = 1;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
  Super::Interact_Implementation(InstigatorPawn);

  if (!ensure(InstigatorPawn)) { return; }

  USAttributeComponent* AttribComp = Cast<USAttributeComponent>(
    InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())
  );

  if (ensure(AttribComp) && !AttribComp->IsFullHealth()) {
    ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
    if (ensure(PS)) {
      if (AttribComp->ApplyHealthChange(this, HealthAmount)) {
        PS->RemoveCredits(CreditsCost);
        Deactivate();
      }
    }
  }
}

FText ASPowerUp_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn) {
  USAttributeComponent* AttribComp = Cast<USAttributeComponent>(
    InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())
  );

  if (ensure(AttribComp) && AttribComp->IsFullHealth()) {
    return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
  }
  return FText::Format(
    LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."),
    CreditsCost
  );
}

#undef LOCTEXT_NAMESPACE
