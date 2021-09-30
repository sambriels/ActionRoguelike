// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

ASBlackholeProjectile::ASBlackholeProjectile() {
  ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
  ForceComp->SetupAttachment(SphereComp);
}


void ASBlackholeProjectile::BeginPlay() {
  Super::BeginPlay();

}

void ASBlackholeProjectile::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

}
