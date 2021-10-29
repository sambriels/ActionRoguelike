#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_LowHealthCheck.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USBTService_LowHealthCheck : public UBTService {
  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, Category="AI")
  FBlackboardKeySelector IsLowHealthKey;

  virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
