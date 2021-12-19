#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter() {
  PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

  ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

  HitFlashTimeParamName = "HitFlashTime";
  HitFlashColorParamName = "HitFlashColor";

  // No longer needed since we have a specifc projectile profile that does this automatically
  // GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
  // GetMesh()->SetGenerateOverlapEvents(true);

}

void ASAICharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();
  PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
  AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget) {
  AAIController* AIC = Cast<AAIController>(GetController());

  if (AIC) {
    UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
    BBComp->SetValueAsObject("TargetActor", NewTarget);
  }
}

AActor* ASAICharacter::GetCurrentTargetActor() const {
  AAIController* AIC = Cast<AAIController>(GetController());

  if (AIC) {
    const UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
    return Cast<AActor>(BBComp->GetValueAsObject("TargetActor"));
  }
  return nullptr;
}

void ASAICharacter::OnHealthChanged(
  AActor* InstigatorActor,
  USAttributeComponent* OwningComp,
  float NewHealth,
  float Delta
) {
  if (Delta < 0.f) {

    if (InstigatorActor != this) {
      SetTargetActor(InstigatorActor);
    }

    if (ActiveHealthBar == nullptr) {
      ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
      if (ActiveHealthBar) {
        ActiveHealthBar->AttachedActor = this;
        ActiveHealthBar->AddToViewport();
      }
    }

    GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
    GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(0, 1, 0));

    // Died
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

      GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
      GetCharacterMovement()->DisableMovement();
    }
  }
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
  if (Cast<APawn>(GetCurrentTargetActor()) != Pawn && ensure(PlayerSpottedWidgetClass)) {
    SetTargetActor(Pawn);

    MultiCastPawnSeen();
  }
}

void ASAICharacter::MultiCastPawnSeen_Implementation() {
  USWorldUserWidget* PlayerSpottedWidget = CreateWidget<USWorldUserWidget>(
    GetWorld(),
    PlayerSpottedWidgetClass
  );
  PlayerSpottedWidget->AttachedActor = this;
  // If no index specified, may end up behind player health bar
  PlayerSpottedWidget->AddToViewport(10);
}
