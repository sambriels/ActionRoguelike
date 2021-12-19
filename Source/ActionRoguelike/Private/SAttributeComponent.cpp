#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

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

  SetIsReplicatedByDefault(true);
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
    return Cast<USAttributeComponent>(
      FromActor->GetComponentByClass(USAttributeComponent::StaticClass())
    );
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

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
  if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) {
    return false;
  }

  if (Delta < 0.0f) {
    float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

    Delta *= DamageMultiplier;
  }

  const float OldHealth = Health;
  const float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

  const float ActualDelta = NewHealth - OldHealth;

  // Is Server?
  if (GetOwner()->HasAuthority()) {
    Health = NewHealth;

    if (ActualDelta != 0.0f) {
      MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
    }

    // Died
    if (ActualDelta < 0.0f && Health == 0.0f) {
      ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
      if (GM) {
        GM->OnActorKilled(GetOwner(), InstigatorActor);
      }
    }
  }

  return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta) {
  const float OldRage = Rage;

  Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

  const float ActualDelta = Rage - OldRage;
  if (ActualDelta != 0.0f) {
    OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
  }

  return ActualDelta != 0;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(
  AActor* InstigatorActor,
  float NewHealth,
  float Delta
) {
  OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(
  AActor* InstigatorActor,
  float NewRage,
  float Delta
) {
  OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(
  TArray<FLifetimeProperty>& OutLifetimeProps
) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(USAttributeComponent, Health);
  DOREPLIFETIME(USAttributeComponent, MaxHealth);

  DOREPLIFETIME(USAttributeComponent, Rage);
  DOREPLIFETIME(USAttributeComponent, MaxRage);

  // DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly);
}
