// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile() {
  MoveComp->InitialSpeed = 6000.f;
}

void ASMagicProjectile::Explode_Implementation() {
  Super::Explode_Implementation();
}
