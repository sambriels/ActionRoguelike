// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

#include "Particles/ParticleSystemComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile() {
  PrimaryActorTick.bCanEverTick = true;

  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  RootComponent = SphereComp;

  ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
  ForceComp->SetupAttachment(SphereComp);

  ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
  ParticleComp->SetupAttachment(SphereComp);

  MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
  MovementComp->InitialSpeed = 1000.0f;
  MovementComp->bRotationFollowsVelocity = true;
  MovementComp->bInitialVelocityInLocalSpace = true;
}


void ASBlackholeProjectile::BeginPlay() {
  Super::BeginPlay();

}

void ASBlackholeProjectile::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

}
