#pragma once

#include "CoreMinimal.h"
#include "SAction.generated.h"

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject {
  GENERATED_BODY()

public:
  /* Action nickname to start/stop without a reference to the object */
  UPROPERTY(EditDefaultsOnly, Category="Action")
  FName ActionName;

  UFUNCTION(BlueprintNativeEvent, Category="Action")
  void StartAction(AActor* Instigator);

  UFUNCTION(BlueprintNativeEvent, Category="Action")
  void StopAction(AActor* Instigator);

  virtual class UWorld* GetWorld() const override;
};
