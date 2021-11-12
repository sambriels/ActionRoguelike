#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"

USActionEffect_Thorns::USActionEffect_Thorns() {
  Duration = 0.f;
  Period = 0.f;

  ReflectionPercent = 0.1;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator) {
  Super::StartAction_Implementation(Instigator);

  USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

  if (AttribComp) {
    AttribComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
  }
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator) {
  Super::StopAction_Implementation(Instigator);

  USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

  if (AttribComp) {
    AttribComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
  }
}

void USActionEffect_Thorns::OnHealthChanged(
  AActor* InstigatorActor,
  USAttributeComponent* OwningComp,
  float NewHealth,
  float Delta
) {
  AActor* Owner = OwningComp->GetOwner();
  if (InstigatorActor == Owner) {
    UE_LOG(LogTemp, Warning, TEXT("Owner is instigator, no throns effect applied"));
    return;
  }

  if (Delta >= 0.f) {
    UE_LOG(LogTemp, Warning, TEXT("Not damage, no thorns"));
    return;
  }

  USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(InstigatorActor);
  if (AttribComp) {
    AttribComp->ApplyHealthChange(Owner, FMath::RoundToInt(Delta * ReflectionPercent));
  }
}
