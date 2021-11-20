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
  float NewHealth = FMath::Clamp<float>(OldHealth + Amount, 0, MaxHealth);
  const float Delta = NewHealth - OldHealth;

  // Is Server? Only then apply actual Health change
  if (GetOwner()->HasAuthority()) {
    Health = NewHealth;

    if (Delta != 0.f) {
      MulticastHealthChanged(InstigatorActor, Health, Delta);
    }

    // Died
    if (Delta <= 0.f && Health == 0.0f) {
      ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();

      if (GM) {
        GM->OnActorKilled(GetOwner(), InstigatorActor);
      }
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

void USAttributeComponent::MulticastHealthChanged_Implementation(
  AActor* InstigatorActor,
  float NewHealth,
  float Delta
) {
  OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void USAttributeComponent::GetLifetimeReplicatedProps(
  TArray<FLifetimeProperty>& OutLifetimeProps
) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(USAttributeComponent, Health);
  DOREPLIFETIME(USAttributeComponent, MaxHealth);

  // DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly);
}
