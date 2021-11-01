// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASCharacter();

  virtual void PostInitializeComponents() override;
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
  virtual FVector GetPawnViewLocation() const override;

  UFUNCTION(Exec)
  void HealSelf(float Amount = 100.f);

protected:
  UPROPERTY(VisibleAnywhere, Category="Components")
  class UCameraComponent* CameraComponent;

  UPROPERTY(VisibleAnywhere, Category="Components")
  class USpringArmComponent* SpringArmComponent;

  UPROPERTY(VisibleAnywhere, Category="Components")
  class USInteractionComponent* InteractionComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
  class USAttributeComponent* AttributeComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
  class USActionComponent* ActionComp;

  UPROPERTY(VisibleAnywhere, Category="Names")
  FName HitFlashTimeParamName;

  UPROPERTY(VisibleAnywhere, Category="Names")
  FName HitFlashColorParamName;

  virtual void BeginPlay() override;

  void MoveForward(float Value);
  void MoveRight(float Value);

  void SprintStart();
  void SprintStop();

  void PrimaryAttack();
  void BlackHole();
  void Dash();

  void PrimaryInteract();

  UFUNCTION()
  void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
