// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class USAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASCharacter();

  virtual void PostInitializeComponents() override;
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
  USAttributeComponent* AttributeComp;

  UPROPERTY(VisibleAnywhere, Category="Components")
  UCameraComponent* CameraComponent;

  UPROPERTY(VisibleAnywhere, Category="Components")
  USpringArmComponent* SpringArmComponent;

  UPROPERTY(VisibleAnywhere, Category="Components")
  USInteractionComponent* InteractionComp;

  UPROPERTY(VisibleAnywhere, Category="Names")
  FName AttackSocketName;

  UPROPERTY(VisibleAnywhere, Category="Names")
  FName HitFlashTimeParamName;

  UPROPERTY(VisibleAnywhere, Category="Names")
  FName HitFlashColorParamName;

  UPROPERTY(EditAnywhere, Category="Attack")
  float AttackAnimDelay;

  UPROPERTY(EditAnywhere, Category="Attack")
  UAnimMontage* AttackAnim;

  UPROPERTY(EditAnywhere, Category="Attack")
  UParticleSystem* CastingEffect;

  UPROPERTY(EditAnywhere, Category="Attack")
  TSubclassOf<AActor> PrimaryAttackProjectileClass;

  UPROPERTY(EditAnywhere, Category="Attack")
  TSubclassOf<AActor> DashProjectileClass;

  UPROPERTY(EditAnywhere, Category="Attack")
  TSubclassOf<AActor> BlackHoleProjectileClass;

  FTimerHandle TimerHandle_PrimaryAttack;
  FTimerHandle TimerHandle_DashProjectile;
  FTimerHandle TimerHandle_BlackHoleProjectile;

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

  void StartAttackEffect();

  UFUNCTION()
  void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
