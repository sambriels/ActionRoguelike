#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASMagicProjectile::ASMagicProjectile() {
  MoveComp->InitialSpeed = 6000.f;
  DamageAmount = 0.f;
  InitialLifeSpan = 10.f;
}

void ASMagicProjectile::BeginPlay() {
  Super::BeginPlay();
  SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex,
  bool bFromSweep,
  const FHitResult& SweepResult
) {
  if (OtherActor && OtherActor != GetInstigator()) {
    USActionComponent* ActionComponent = Cast<USActionComponent>(
      OtherActor->GetComponentByClass(USActionComponent::StaticClass())
    );

    if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag)) {
      MoveComp->Velocity = -MoveComp->Velocity;

      SetInstigator(Cast<APawn>(OtherActor));
      return;
    }

    if (USGameplayFunctionLibrary::ApplyDirectionDamage(
      GetInstigator(),
      OtherActor,
      DamageAmount,
      SweepResult
    )) {
      Explode();
      if (ActionComponent && HasAuthority()) {
        ActionComponent->AddAction(GetInstigator(), BurningActionClass);
      }
    }
  }
}
