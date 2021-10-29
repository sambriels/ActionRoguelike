#include "AI/SBTService_LowHealthCheck.h"

#include "AIController.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_LowHealthCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
  // Check if we are low health

  UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

  if (ensure(BlackboardComp)) {
    AAIController* AIC = OwnerComp.GetAIOwner();

    if (ensure(AIC)) {
      AActor* AIActor = Cast<AActor>(AIC->GetPawn());

      if (AIActor) {
        USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIActor);

        if (AttributeComp) {
          BlackboardComp->SetValueAsBool(IsLowHealthKey.SelectedKeyName, AttributeComp->IsLowHealth());
        }
      }
    }
  }
}
