// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPickUpBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPickUpBase : public AActor, public ISGameplayInterface {
  GENERATED_BODY()

public:
  ASPickUpBase();

  virtual void Interact_Implementation(APawn* InstigatorPawn) override;
  // virtual void Tick(float DeltaTime) override;
protected:
  UPROPERTY(EditDefaultsOnly, Category="Components")
  USphereComponent* SphereComp;

  UPROPERTY(EditDefaultsOnly, Category="Components")
  UStaticMeshComponent* MeshComp;

  // virtual void BeginPlay() override;

};
