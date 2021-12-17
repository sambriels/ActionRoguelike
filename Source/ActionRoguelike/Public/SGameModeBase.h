// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USMonsterData;
class UDataTable;
class USSaveGame;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class UCurveFloat;

/* DataTable Row for spawning monsters in game mode */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase {
  GENERATED_BODY()
public:
  FMonsterInfoRow() {
    Weight = 1.0f;
    SpawnCost = 5.0f;
    KillReward = 20.0f;
  }

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FPrimaryAssetId MonsterId;

  /* Relative chance to pick this monster */
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float Weight;

  /* Points required by game mode to pic this monster */
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float SpawnCost;

  /* Amount of credits awarded to killer of this unit */
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float KillReward;
};

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase {
  GENERATED_BODY()

public:
  ASGameModeBase();
  virtual void StartPlay() override;

  virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

  virtual void InitGame(
    const FString& MapName,
    const FString& Options,
    FString& ErrorMessage
  ) override;

  virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

  UFUNCTION(Exec)
  void KillAll();

  UFUNCTION(BlueprintCallable, Category="Save Game")
  void WriteSaveGame();

  void LoadSaveGame();

protected:
  FString SlotName;

  UPROPERTY()
  USSaveGame* CurrentSaveGame;

  /* All available monsters */
  UPROPERTY(EditDefaultsOnly, Category="AI")
  UDataTable* MonsterTable;

  FTimerHandle TimerHandle_SpawnBots;

  UPROPERTY(EditDefaultsOnly, Category="AI")
  float SpawnTimerInterval;

  UPROPERTY(EditDefaultsOnly, Category="AI")
  UEnvQuery* SpawnBotQuery;

  // Now derived from the monster data table
  // UPROPERTY(EditDefaultsOnly, Category="AI")
  // TSubclassOf<AActor> MinionClass;

  UPROPERTY(EditDefaultsOnly, Category="AI")
  UCurveFloat* DifficultyCurve;

  UPROPERTY(EditDefaultsOnly, Category="Credits")
  int32 CreditsPerKill;

  UPROPERTY(EditDefaultsOnly, Category="PowerUps")
  TArray<TSubclassOf<ASPowerUpBase>> PowerUpClasses;

  UPROPERTY(EditDefaultsOnly, Category="PowerUps")
  UEnvQuery* PowerUpSpawnQuery;

  UPROPERTY(EditDefaultsOnly, Category="PowerUps")
  int32 DesiredPowerUpSpawnCount;;

  UPROPERTY(EditDefaultsOnly, Category="PowerUps")
  float MinimumSpawnDistanceFromOrigin;


  UFUNCTION()
  void RespawnPlayerTimerElapsed(AController* Controller);

  UFUNCTION()
  void SpawnBotTimerElapsed();

  UFUNCTION()
  void OnBotSpawnQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper* QueryInstance,
    EEnvQueryStatus::Type QueryStatus
  );

  void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

  UFUNCTION()
  void OnPowerUpSpawnQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper* QueryInstance,
    EEnvQueryStatus::Type QueryStatus
  );
};
