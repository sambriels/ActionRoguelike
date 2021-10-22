#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent() {
  MaxHealth = 100;
  Health = MaxHealth;
}

bool USAttributeComponent::IsAlive() const {
  return Health > 0.f;
}

bool USAttributeComponent::IsFullHealth() const {
  return Health == MaxHealth;
}

float USAttributeComponent::GetMaxHealth() const {
  return MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(const float Amount) {
  const float OldHealth = Health;
  Health = FMath::Clamp<float>(OldHealth + Amount, 0, MaxHealth);

  const float Delta = Health - OldHealth;
  OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

  return Delta != 0;
}
