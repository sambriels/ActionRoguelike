// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SBlackholeProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBlackholeProjectile : public AActor {
  GENERATED_BODY()

public:
  ASBlackholeProjectile();

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  USphereComponent* SphereComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  URadialForceComponent* ForceComp;

  UPROPERTY(VisibleAnywhere)
  UProjectileMovementComponent* MovementComp;

  UPROPERTY(VisibleAnywhere)
  UParticleSystemComponent* ParticleComp;

  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;

};
