// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile() {
  MoveComp->InitialSpeed = 6000.f;

  DamageAmount = 0.f;
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
    USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(
      OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())
    );
    if (AttributeComp) {
      AttributeComp->ApplyHealthChange(DamageAmount);
      Destroy();
    }
  }
}

void ASMagicProjectile::BeginPlay() {
  Super::BeginPlay();
  SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}
