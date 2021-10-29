#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_Heal.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USBTTask_Heal : public UBTTaskNode {
  GENERATED_BODY()

  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
