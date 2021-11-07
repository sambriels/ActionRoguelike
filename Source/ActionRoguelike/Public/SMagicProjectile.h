// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionEffect.h"
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
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
  float DamageAmount;

  UPROPERTY(EditDefaultsOnly, Category="Damage")
  FGameplayTag ParryTag;

  UPROPERTY(EditDefaultsOnly, Category="Damage")
  TSubclassOf<USActionEffect> BurningActionClass;

  virtual void BeginPlay() override;

  UFUNCTION()
  void OnActorOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
  );
};
