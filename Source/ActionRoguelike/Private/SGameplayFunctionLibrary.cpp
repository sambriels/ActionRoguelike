#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount) {
  USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);

  if (AttributeComponent) {
    return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
  }
  return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionDamage(
  AActor* DamageCauser,
  AActor* TargetActor,
  float DamageAmount,
  const FHitResult& HitResult
) {
  if (ApplyDamage(DamageCauser, TargetActor, DamageAmount)) {
    UPrimitiveComponent* HitComp = HitResult.GetComponent();
    if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName)) {
      HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
    }
    return true;
  }
  return false;
}
