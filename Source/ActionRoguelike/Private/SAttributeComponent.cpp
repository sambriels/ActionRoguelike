#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent() {
  Health = 100;
}

bool USAttributeComponent::ApplyHealthChange(float Delta) {
  Health += Delta;

  OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

  return true;
}
