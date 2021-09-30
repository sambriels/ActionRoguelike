// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBase {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASMagicProjectile();

protected:
  virtual void Explode_Implementation() override;
};
