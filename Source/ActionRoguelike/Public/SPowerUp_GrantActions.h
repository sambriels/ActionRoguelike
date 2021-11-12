#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SPowerUp_GrantActions.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_GrantActions : public ASPowerUpBase {
  GENERATED_BODY()

protected:
  UPROPERTY(EditDefaultsOnly, Category="PowerUp")
  TArray<TSubclassOf<USAction>> ActionClasses;

  virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
