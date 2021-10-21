#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent() {
  MaxHealth = 100;
  Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const {
  return Health > 0.f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta) {
  Health = FMath::Clamp<float>(Health + Delta, 0, MaxHealth);
  OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

  return true;
}
