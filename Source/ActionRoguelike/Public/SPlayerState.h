#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
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
  int32 GetCredits() const;

  UPROPERTY(BlueprintAssignable)
  FOnCreditsChanged OnCreditsChanged;

  UFUNCTION(BlueprintNativeEvent)
  void SavePlayerState(USSaveGame* SaveObject);

  UFUNCTION(BlueprintNativeEvent)
  void LoadPlayerState(USSaveGame* SaveObject);

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category="Credits")
  int32 Credits;

  UFUNCTION()
  void OnRep_Credits(int32 OldCredits);
};
