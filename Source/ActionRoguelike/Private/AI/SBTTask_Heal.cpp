// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  // return Super::ExecuteTask(OwnerComp, NodeMemory);

  AAIController* AIC = OwnerComp.GetAIOwner();

  if (ensure(AIC)) {
    AActor* AIActor = AIC->GetPawn();
    USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIActor);

    if (AttributeComp) {
      AttributeComp->ApplyHealthChange(AIActor, AttributeComp->GetMaxHealth());
      return EBTNodeResult::Succeeded;
    }
  }
  return EBTNodeResult::Failed;
}
