#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject {
  GENERATED_BODY()

public:
  /* Action nickname to start/stop without a reference to the object */
  UPROPERTY(EditDefaultsOnly, Category="Action")
  FName ActionName;

  UFUNCTION(BlueprintNativeEvent, Category="Action")
  bool CanStart(AActor* Instigator);

  UFUNCTION(BlueprintNativeEvent, Category="Action")
  void StartAction(AActor* Instigator);

  UFUNCTION(BlueprintNativeEvent, Category="Action")
  void StopAction(AActor* Instigator);

  virtual class UWorld* GetWorld() const override;

  UFUNCTION(BlueprintCallable, Category="Action")
  bool IsRunning() const;

protected:
  bool bIsRunning;

  UFUNCTION(BlueprintCallable, Category="Action")
  class USActionComponent* GetOwningComponent() const;

  /* Tag added to owning actor when activated, removed when actions stops */
  UPROPERTY(EditDefaultsOnly, Category="Tags")
  FGameplayTagContainer GrantsTags;

  /* Action can only start if owning actor has none of these tags applied */
  UPROPERTY(EditDefaultsOnly, Category="Tags")
  FGameplayTagContainer BlockedTags;
};
