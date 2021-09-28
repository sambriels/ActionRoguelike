// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "PhysicsEngine//RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel() {
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  MeshComp->SetSimulatePhysics(true);
  RootComponent = MeshComp;

  ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
  ForceComp->SetupAttachment(MeshComp);
  ForceComp->Radius = 750.f;
  ForceComp->ImpulseStrength = 2000.f;
  ForceComp->bImpulseVelChange = true;
  ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay() {
  Super::BeginPlay();
}

void ASExplosiveBarrel::PostInitializeComponents() {
  Super::PostInitializeComponents();
  MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(
  UPrimitiveComponent* HitComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  FVector NormalImpulse,
  const FHitResult& Hit
) {
  ForceComp->FireImpulse();
}
