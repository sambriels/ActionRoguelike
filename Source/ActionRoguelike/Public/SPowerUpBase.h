#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPowerUpBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerUpBase : public AActor, public ISGameplayInterface {
  GENERATED_BODY()

public:
  ASPowerUpBase();

  virtual void Interact_Implementation(APawn* InstigatorPawn) override;

  virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
protected:
  UPROPERTY(EditAnywhere, Category="Components")
  USphereComponent* SphereComp;

  UPROPERTY(EditAnywhere, Category="Components")
  UStaticMeshComponent* MeshComp;

  UPROPERTY(EditAnywhere, Category="PowerUp")
  float RespawnInterval;

  UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_IsActive")
  bool bIsActive;

  UFUNCTION()
  void OnRep_IsActive();

  UFUNCTION()
  void Deactivate();

  void Reactivate();
};
