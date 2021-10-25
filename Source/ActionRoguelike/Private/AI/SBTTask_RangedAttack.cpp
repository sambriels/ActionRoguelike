#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTask_RangedAttack::USBTTask_RangedAttack() {
  MaxBulletSpread = 2.f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
  AAIController* MyController = OwnerComp.GetAIOwner();

  if (ensure(MyController)) {
    ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

    if (MyPawn == nullptr) {
      return EBTNodeResult::Failed;
    }

    FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

    if (TargetActor == nullptr) {
      return EBTNodeResult::Failed;
    }

    if (!USAttributeComponent::IsActorAlive(TargetActor)) {
      return EBTNodeResult::Failed;
    }

    FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
    FRotator MuzzleRotation = Direction.Rotation();

    // Adjusts aim of bots, so it does not always hit us
    MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
    MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Instigator = MyPawn;

    AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

    return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
  }

  return EBTNodeResult::Failed;
}
