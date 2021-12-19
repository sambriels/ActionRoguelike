#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
  FOnHealthChanged,
  AActor*,
  InstigatorActor,
  USAttributeComponent*,
  OwningComp,
  float,
  NewHealth,
  float,
  Delta
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
  FOnRageChanged,
  AActor*,
  InstigatorActor,
  USAttributeComponent*,
  OwningComp,
  float,
  NewRage,
  float,
  Delta
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USAttributeComponent();

  UFUNCTION(BlueprintCallable, Category="Attributes")
  static USAttributeComponent* GetAttributes(AActor* FromActor);

  UFUNCTION(BlueprintCallable, Category="Attributes", meta = (DisplayName = "IsAlive"))
  static bool IsActorAlive(AActor* Actor);

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
  float Health;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category="Attributes")
  float MaxHealth;

  UFUNCTION(NetMulticast, Reliable)
  // @FIXME: Mark as unreliable once we moved the state out of SCharacter
  void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

  UFUNCTION(NetMulticast, Unreliable)
  // Used for cosmetic only, therefore marked as unreliable
  void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
  float LowHealthPercentage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
  float Rage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category="Attributes")
  float MaxRage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
  float RageMultiplier;

public:
  UFUNCTION(BlueprintCallable)
  bool Kill(AActor* InstigatorActor);

  UFUNCTION(BlueprintCallable)
  bool IsAlive() const;

  UFUNCTION(BlueprintCallable)
  bool IsLowHealth() const;

  UFUNCTION(BlueprintCallable)
  bool IsFullHealth() const;

  UFUNCTION(BlueprintCallable)
  float GetMaxHealth() const;

  UPROPERTY(BlueprintAssignable)
  FOnHealthChanged OnHealthChanged;

  UPROPERTY(BlueprintAssignable)
  FOnRageChanged OnRageChanged;

  UFUNCTION(BlueprintCallable, Category="Attributes")
  bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

  UFUNCTION(BlueprintCallable, Category="Attributes")
  bool ApplyRageChange(AActor* InstigatorActor, float Delta);
};
