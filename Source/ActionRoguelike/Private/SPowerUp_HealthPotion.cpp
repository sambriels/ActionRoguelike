#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() {
  HealthAmount = 20.f;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
  Super::Interact_Implementation(InstigatorPawn);

  if (ensure(InstigatorPawn)) {
    USAttributeComponent* AttribComp = Cast<USAttributeComponent>(
      InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())
    );

    if (AttribComp->ApplyHealthChange(this, HealthAmount)) {
      Deactivate();
    }
  }
}
