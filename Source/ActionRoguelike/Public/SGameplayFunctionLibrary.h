#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, Category="Gameplay")
  static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

  UFUNCTION(BlueprintCallable, Category="Gameplay")
  static bool ApplyDirectionDamage(
    AActor* DamageCauser,
    AActor* TargetActor,
    float DamageAmount,
    // if not passed as const, it becomes an output pin in blueprint instead of an input pin.
    // This is the case for all args passed by reference
    const FHitResult& HitResult
  );
};
