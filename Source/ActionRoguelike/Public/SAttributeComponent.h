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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USAttributeComponent();

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
  float Health;

public:
  UFUNCTION(BlueprintCallable)
  bool IsAlive() const;

  UPROPERTY(BlueprintAssignable)
  FOnHealthChanged OnHealthChanged;

  UFUNCTION(BlueprintCallable, Category="Attributes")
  bool ApplyHealthChange(float Delta);
};
