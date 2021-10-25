#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASAICharacter();

protected:
  virtual void PostInitializeComponents() override;

  UPROPERTY(VisibleAnywhere, Category="Components")
  UPawnSensingComponent* PawnSensingComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
  USAttributeComponent* AttributeComp;

  UFUNCTION()
  void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

  UFUNCTION()
  void OnPawnSeen(APawn* Pawn);
};
