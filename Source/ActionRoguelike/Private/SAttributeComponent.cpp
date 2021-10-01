#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent() {
  Health = 100;
}

bool USAttributeComponent::IsAlive() const {
  return Health > 0.f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta) {
  Health += Delta;

  OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

  return true;
}
