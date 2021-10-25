#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

  // check distance between AI pawn and target actor

  UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
  if (ensure(BlackboardComp)) {
    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

    if (TargetActor) {
      AAIController* MyController = OwnerComp.GetAIOwner();

      if (ensure(MyController)) {
        APawn* AIPawn = MyController->GetPawn();

        if (ensure(AIPawn)) {
          const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

          const bool bWithinRange = DistanceTo < 2000.f;
          bool bHasLineOfSight = false;
          if (bWithinRange) {
            bHasLineOfSight = MyController->LineOfSightTo(TargetActor);
          }

          BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLineOfSight));
        }
      }
    }
  }
}
