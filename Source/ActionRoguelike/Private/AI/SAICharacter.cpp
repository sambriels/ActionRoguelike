#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
}

void ASAICharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();
  PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
  AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnHealthChanged(
  AActor* InstigatorActor,
  USAttributeComponent* OwningComp,
  float NewHealth,
  float Delta
) {
  if (Delta < 0.f) {
    if (NewHealth <= 0.f) {
      // Stop behaviour tree
      AAIController* AIC = Cast<AAIController>(GetController());
      if (AIC) {
        AIC->GetBrainComponent()->StopLogic("Killed");
      }

      // Ragdoll
      GetMesh()->SetAllBodiesSimulatePhysics(true);
      GetMesh()->SetCollisionProfileName("Ragdoll");
      // set lifespan (how long untill destroy actor is called
      SetLifeSpan(10.f);

    }
  }
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
  AAIController* AIC = Cast<AAIController>(GetController());

  if (AIC) {
    UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

    BBComp->SetValueAsObject("TargetActor", Pawn);

    // DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
  }
}
