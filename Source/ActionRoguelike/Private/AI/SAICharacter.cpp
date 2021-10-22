#include "AI/SAICharacter.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

void ASAICharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();
  PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
  AAIController* AIC = Cast<AAIController>(GetController());

  if (AIC) {
    UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

    BBComp->SetValueAsObject("TargetActor", Pawn);

    DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
  }
}
