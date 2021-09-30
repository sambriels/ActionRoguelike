// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter {
  GENERATED_BODY()

protected:
  UPROPERTY(EditAnywhere, Category="Attack")
  TSubclassOf<AActor> PrimaryAttackProjectileClass;

  UPROPERTY(EditAnywhere, Category="Dash Projectile")
  TSubclassOf<AActor> DashProjectileClass;

  UPROPERTY(EditAnywhere, Category="Attack")
  UAnimMontage* AttackAnim;

  UPROPERTY(EditAnywhere, Category="BlackHole")
  TSubclassOf<AActor> BlackHoleProjectileClass;

  FTimerHandle TimerHandle_PrimaryAttack;
  FTimerHandle TimerHandle_DashProjectile;
  FTimerHandle TimerHandle_BlackHoleProjectile;

public:
  // Sets default values for this character's properties
  ASCharacter();

protected:
  UPROPERTY(VisibleAnywhere)
  UCameraComponent* CameraComponent;

  UPROPERTY(VisibleAnywhere)
  USpringArmComponent* SpringArmComponent;

  UPROPERTY(VisibleAnywhere)
  USInteractionComponent* InteractionComp;

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void MoveForward(float Value);
  void MoveRight(float Value);

  void PrimaryAttack();
  void PrimaryAttack_TimeElapsed();

  void BlackHoleProjectile();
  void BlackHoleProjectile_TimeElapsed();

  void DashProjectile();
  void DashProjectile_TimeElapsed();

  void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);

  void PrimaryInteract();

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
