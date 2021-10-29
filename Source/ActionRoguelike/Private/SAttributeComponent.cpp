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
  UE_LOG(
    LogTemp,
    Warning,
    TEXT("Is low health: %d, %i, %i, %i"),
    Health / MaxHealth <= LowHealthPercentage,
    Health,
    MaxHealth,
    LowHealthPercentage
  );
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
