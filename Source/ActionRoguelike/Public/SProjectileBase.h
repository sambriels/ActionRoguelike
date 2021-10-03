// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SProjectileBase.generated.h"

// Abstract marks this class as incomplete, so it won't show in certain dropdown windows like SpawnActor etc
UCLASS(Abstract)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor {
  GENERATED_BODY()

public:
  ASProjectileBase();

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
  UParticleSystem* ImpactVfx;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
  USphereComponent* SphereComp;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
  UProjectileMovementComponent* MoveComp;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
  UParticleSystemComponent* VfxComp;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
  UAudioComponent* AudioComp;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
  USoundBase* ImpactSound;

  // 'virtual' so we can override this in child classes
  UFUNCTION()
  virtual void OnActorHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit
  );

  // BlueprintNativeEvent: C++ base implementation, can be expanded in BP
  // BlueprintCallable to allow child classes to trigger explosions from BP
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
  void Explode();

  virtual void BeginPlay() override;
  virtual void PostInitializeComponents() override;
};
