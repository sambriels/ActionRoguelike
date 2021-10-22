#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay() {
  Super::BeginPlay();

  if (ensureMsgf(BehaviourTree, TEXT("BehaviourTree is nullptr! Please assign BehaviourTree in your AI controller"))) {
    RunBehaviorTree(BehaviourTree);
  }


  // APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
  // if (MyPawn) {
  //   GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
  //
  //   GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
  // }
}
