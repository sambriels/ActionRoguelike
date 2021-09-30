// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase {
  GENERATED_BODY()

public:
  ASDashProjectile();

protected:
  UPROPERTY(EditAnywhere)
  float TeleportDelay;

  UPROPERTY(EditAnywhere)
  float DetonationDelay;

  FTimerHandle TimerHandle_Detonation;

  void TeleportInstigator();

  virtual void BeginPlay() override;
  virtual void Explode_Implementation() override;

};
