#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SCharacter.h"
#include "SAction_ProjectileAttack.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction {
  GENERATED_BODY()

public:
  USAction_ProjectileAttack();

  virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
  UPROPERTY(VisibleAnywhere, Category="Names")
  FName AttackSocketName;

  UPROPERTY(EditAnywhere, Category="Attack")
  float AttackAnimDelay;

  UPROPERTY(EditAnywhere, Category="Attack")
  UAnimMontage* AttackAnim;

  UPROPERTY(EditAnywhere, Category="Attack")
  UParticleSystem* CastingEffect;

  UPROPERTY(EditAnywhere, Category="Attack")
  TSubclassOf<AActor> ProjectileClass;

  UFUNCTION()
  void AttackDelay_Elapsed(ASCharacter* InstigatorCharacter);
};
