#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
  FOnCreditsChanged,
  class ASPlayerState*,
  PlayerState,
  int32,
  NewCredits,
  int32,
  Delta
);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState {
  GENERATED_BODY()

public:
  ASPlayerState();

  UFUNCTION(BlueprintCallable, Category="Credits")
  void AddCredits(int32 Delta);

  UFUNCTION(BlueprintCallable, Category="Credits")
  void RemoveCredits(int32 Delta);

  UFUNCTION(BlueprintCallable, Category="Credits")
  int32 GetCredits();

  UPROPERTY(BlueprintAssignable)
  FOnCreditsChanged OnCreditsChanged;

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, Category="Credits")
  int32 Credits;
};
