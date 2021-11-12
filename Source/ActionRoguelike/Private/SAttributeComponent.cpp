#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
  TEXT("su.DamageMultiplier"),
  1.f,
  TEXT("Global Damage multiplier for Attribute Component"),
  ECVF_Cheat
);

USAttributeComponent::USAttributeComponent() {
  MaxHealth = 100;
  Health = MaxHealth;
  LowHealthPercentage = 0.5f;

  Rage = 0.f;
  MaxRage = 80.f;
  RageMultiplier = 1.3;
}


bool USAttributeComponent::IsActorAlive(AActor* Actor) {
  USAttributeComponent* AttributeComp = GetAttributes(Actor);

  if (AttributeComp) {
    return AttributeComp->IsAlive();
  }

  return false;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor) {
  if (FromActor) {
    return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
  }
  return nullptr;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor) {
  return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USAttributeComponent::IsAlive() const {
  return Health > 0.f;
}

bool USAttributeComponent::IsLowHealth() const {
  return Health / MaxHealth <= LowHealthPercentage;
}

bool USAttributeComponent::IsFullHealth() const {
  return Health == MaxHealth;
}

float USAttributeComponent::GetMaxHealth() const {
  return MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Amount) {
  if (!GetOwner()->CanBeDamaged() && Amount < 0.f) {
    return false;
  }

  if (Amount < 0.f) {
    float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
    Amount *= DamageMultiplier;

    ApplyRageChange(FMath::Abs(Amount) * RageMultiplier);
  }

  const float OldHealth = Health;
  Health = FMath::Clamp<float>(OldHealth + Amount, 0, MaxHealth);

  const float Delta = Health - OldHealth;
  OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

  // Died
  if (Delta <= 0.f && Health == 0.0f) {
    ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();

    if (GM) {
      GM->OnActorKilled(GetOwner(), InstigatorActor);
    }
  }

  return Delta != 0;
}

void USAttributeComponent::ApplyRageChange(float Amount) {
  const float OldRage = Rage;
  const float RoundedValue = FMath::Floor(OldRage + Amount);
  Rage = FMath::Clamp<float>(RoundedValue, 0, MaxRage);

  const float Delta = Rage + OldRage;
  OnRageChanged.Broadcast(this, Rage, Delta);
}
