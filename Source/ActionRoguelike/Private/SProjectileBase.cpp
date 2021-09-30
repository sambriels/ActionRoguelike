#include "SProjectileBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASProjectileBase::ASProjectileBase() {
  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  SphereComp->SetCollisionProfileName("Projectile");
  SphereComp->SetSphereRadius(1.f);
  SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
  RootComponent = SphereComp;

  VfxComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFXComp"));
  VfxComp->SetupAttachment(SphereComp);

  MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
  MoveComp->bRotationFollowsVelocity = true;
  MoveComp->bInitialVelocityInLocalSpace = true;
  MoveComp->ProjectileGravityScale = 0.f;
  MoveComp->InitialSpeed = 8000.f;
}

void ASProjectileBase::OnActorHit(
  UPrimitiveComponent* HitComp,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  FVector NormalImpulse,
  const FHitResult& Hit
) {
  // Still call it without _Implementation from within C++
  Explode();
}

// _Implementation from it being marked as BlueprintNativeEvent
void ASProjectileBase::Explode_Implementation() {
  // Make sure we are not already in the process of being destroyed
  // Adding ensure to see if this triggers at all
  if (ensure(!IsPendingKill())) {
    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());
    Destroy();
  }
}

void ASProjectileBase::PostInitializeComponents() {
  Super::PostInitializeComponents();
  SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}
