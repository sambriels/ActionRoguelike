#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SPowerUp_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_Credits : public ASPowerUpBase {
  GENERATED_BODY()

public:
  ASPowerUp_Credits();

protected:
  UPROPERTY(EditDefaultsOnly, Category="Credits")
  int32 CreditsAmount;

  virtual void Interact_Implementation(APawn* InstigatorPawn) override;

};
