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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
  FOnRageChanged,
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
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
  float Health;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
  float MaxHealth;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
  float LowHealthPercentage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
  float Rage;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
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
  bool ApplyHealthChange(AActor* InstigatorActor, float Amount);

  UFUNCTION(BlueprintCallable, Category="Attributes")
  void ApplyRageChange(float Amount);
};
