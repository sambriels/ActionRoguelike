// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp_HealthPotion.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() {
  HealthAmount = 20.f;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
  Super::Interact_Implementation(InstigatorPawn);

  if (ensure(InstigatorPawn)) {
    USAttributeComponent* AttribComp = Cast<USAttributeComponent>(
      InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())
    );

    if (AttribComp->ApplyHealthChange(HealthAmount)) {
      Deactivate();
    }
  }
}
