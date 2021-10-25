#include "SDashProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile() {
  TeleportDelay = 0.2f;
  DetonationDelay = 0.2f;
}

void ASDashProjectile::TeleportInstigator() {
  AActor* ActorToTeleport = GetInstigator();

  if (ensure(ActorToTeleport)) {
    ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
    Destroy();
  }
}

void ASDashProjectile::BeginPlay() {
  Super::BeginPlay();
  GetWorldTimerManager().SetTimer(TimerHandle_Detonation, this, &ASDashProjectile::Explode, DetonationDelay);
}

void ASDashProjectile::Explode_Implementation() {
  GetWorldTimerManager().ClearTimer(TimerHandle_Detonation);

  VfxComp->DeactivateSystem();
  MoveComp->StopMovementImmediately();
  SetActorEnableCollision(false);

  UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());

  FTimerHandle TimerHandle_Teleport;
  GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
  // No super call, as that would destroy the actor too soon
  // Super::Explode_Implementation();
}
