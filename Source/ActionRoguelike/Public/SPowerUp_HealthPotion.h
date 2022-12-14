// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SPowerUp_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_HealthPotion : public ASPowerUpBase {
  GENERATED_BODY()

public:
  ASPowerUp_HealthPotion();

  virtual void Interact_Implementation(APawn* InstigatorPawn) override;
  virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
protected:
  UPROPERTY(EditAnywhere, Category="HealthPotion")
  float HealthAmount;

  UPROPERTY(EditDefaultsOnly, Category="HealthPotion")
  int32 CreditsCost;

};
