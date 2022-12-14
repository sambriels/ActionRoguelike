#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"
class USAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect {
  GENERATED_BODY()

public:
  USActionEffect_Thorns();

  virtual void StartAction_Implementation(AActor* Instigator) override;
  virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
  UPROPERTY(EditDefaultsOnly, Category="Effect")
  float ReflectionPercent;

  UFUNCTION()
  void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
