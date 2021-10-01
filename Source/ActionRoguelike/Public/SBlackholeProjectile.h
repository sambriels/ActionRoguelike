// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SBlackholeProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBlackholeProjectile : public ASProjectileBase {
  GENERATED_BODY()

public:
  ASBlackholeProjectile();

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  URadialForceComponent* ForceComp;
};
