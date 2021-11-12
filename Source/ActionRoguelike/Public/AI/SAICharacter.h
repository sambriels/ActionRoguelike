#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASAICharacter();

protected:
  UPROPERTY()
  class USWorldUserWidget* ActiveHealthBar;

  UPROPERTY(EditDefaultsOnly, Category="UI")
  TSubclassOf<UUserWidget> HealthBarWidgetClass;

  UPROPERTY(EditDefaultsOnly, Category="UI")
  TSubclassOf<UUserWidget> PlayerSpottedWidgetClass;

  UPROPERTY(VisibleAnywhere, Category="Effect")
  FName HitFlashTimeParamName;

  UPROPERTY(VisibleAnywhere, Category="Effect")
  FName HitFlashColorParamName;

  UPROPERTY(VisibleAnywhere, Category="Components")
  class UPawnSensingComponent* PawnSensingComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
  class USAttributeComponent* AttributeComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
  class USActionComponent* ActionComp;

  virtual void PostInitializeComponents() override;

  void SetTargetActor(AActor* NewTarget);

  AActor* GetCurrentTargetActor() const;

  UFUNCTION()
  void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

  UFUNCTION()
  void OnPawnSeen(APawn* Pawn);

};
