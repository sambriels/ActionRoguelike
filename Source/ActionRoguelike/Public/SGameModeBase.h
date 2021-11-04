// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase {
  GENERATED_BODY()

public:
  ASGameModeBase();
  virtual void StartPlay() override;

  virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

  UFUNCTION(Exec)
  void KillAll();

protected:
  FTimerHandle TimerHandle_SpawnBots;

  UPROPERTY(EditDefaultsOnly, Category="AI")
  float SpawnTimerInterval;

  UPROPERTY(EditDefaultsOnly, Category="AI")
  UEnvQuery* SpawnBotQuery;

  UPROPERTY(EditDefaultsOnly, Category="AI")
  TSubclassOf<AActor> MinionClass;

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
  void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

  UFUNCTION()
  void OnPowerUpSpawnQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper* QueryInstance,
    EEnvQueryStatus::Type QueryStatus
  );
};
