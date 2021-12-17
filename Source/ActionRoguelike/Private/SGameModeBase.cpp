#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/SAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
  TEXT("su.SpawnBots"),
  false,
  TEXT("Enable spawning of bots via timer."),
  ECVF_Cheat
);

ASGameModeBase::ASGameModeBase() {
  SpawnTimerInterval = 2.f;
  CreditsPerKill = 1;

  DesiredPowerUpSpawnCount = 20;
  MinimumSpawnDistanceFromOrigin = 200.f;

  PlayerStateClass = ASPlayerState::StaticClass();

  SlotName = "SaveGame01";
}

void ASGameModeBase::StartPlay() {
  Super::StartPlay();

  // Continuous timer to spawn bots, can be limited in SpawnBotTimerElapsed
  GetWorldTimerManager().SetTimer(
    TimerHandle_SpawnBots,
    this,
    &ASGameModeBase::SpawnBotTimerElapsed,
    SpawnTimerInterval,
    true
  );

  if (ensure(PowerUpClasses.Num() > 0)) {
    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
      this,
      PowerUpSpawnQuery,
      this,
      EEnvQueryRunMode::AllMatching,
      nullptr
    );
    if (ensure(QueryInstance)) {
      QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
        this,
        &ASGameModeBase::OnPowerUpSpawnQueryCompleted
      );
    }
  }

}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) {
  ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
  if (PS) {
    PS->LoadPlayerState(CurrentSaveGame);
  }

  Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::InitGame(
  const FString& MapName,
  const FString& Options,
  FString& ErrorMessage
) {
  Super::InitGame(MapName, Options, ErrorMessage);

  LoadSaveGame();
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
  ASCharacter* Player_Victim = Cast<ASCharacter>(VictimActor);
  if (Player_Victim) {
    FTimerHandle TimerHandle_RespawnDelay;

    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "RespawnPlayerTimerElapsed", Player_Victim->GetController());

    constexpr float RespawnDelay = 2.f;
    GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
  } else {
    const APawn* KillerPawn = Cast<APawn>(Killer);
    if (KillerPawn) {
      ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
      if (PS) {
        PS->AddCredits(CreditsPerKill);
      }
    }
  }

  UE_LOG(
    LogTemp,
    Log,
    TEXT("OnActorKilled: Victim: %s, Killer: %s"),
    *GetNameSafe(VictimActor),
    *GetNameSafe(Killer)
  );
}

void ASGameModeBase::KillAll() {
  for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
    ASAICharacter* Bot = *It;

    USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
    if (ensure(AttribComp) && AttribComp->IsAlive()) {
      AttribComp->Kill(this); // TODO: Pass in player? for kill credit?
    }
  }
}

void ASGameModeBase::WriteSaveGame() {
  /** Iterate over all player states, we don't have aproper IDs to match yet (requires Steam or EOS */
  for (int32 i = 0; i < GameState->PlayerArray.Num(); i++) {
    ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
    if (PS) {
      PS->SavePlayerState(CurrentSaveGame);
      break; // Single player only at this point
    }
  }

  CurrentSaveGame->SavedActors.Empty();

  for (FActorIterator It(GetWorld()); It; ++It) {
    AActor* Actor = *It;

    // Only store our "gameplay" actors
    if (!Actor->Implements<USGameplayInterface>()) {
      continue;
    }

    FActorSaveData ActorData;
    ActorData.ActorName = Actor->GetName();
    ActorData.Transform = Actor->GetActorTransform();

    // Pass array of bytes to fill with data from actor
    FMemoryWriter MemWriter(ActorData.BytesData);

    FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
    // Find only variables marked with UPROPERTY(SaveGame)
    Ar.ArIsSaveGame = true;
    // Convert Actor's SaveGame UPROPERTIES into binary array
    Actor->Serialize(Ar);

    CurrentSaveGame->SavedActors.Add(ActorData);
  }

  UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame() {
  if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
    CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!CurrentSaveGame) {
      UE_LOG(LogTemp, Warning, TEXT("Failed to load save game data"));
      return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Loaded save game data"));

    for (FActorIterator It(GetWorld()); It; ++It) {
      AActor* Actor = *It;

      // Only store our "gameplay" actors
      if (!Actor->Implements<USGameplayInterface>()) {
        continue;
      }

      for (FActorSaveData ActorData : CurrentSaveGame->SavedActors) {
        if (ActorData.ActorName == Actor->GetName()) {
          Actor->SetActorTransform(ActorData.Transform);

          // Read binary array of Saved UPROPRTIES
          FMemoryReader MemReader(ActorData.BytesData);

          FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
          Ar.ArIsSaveGame = true;
          // Convert binary data back into Actor's variables
          Actor->Serialize(Ar);

          ISGameplayInterface::Execute_OnActorLoaded(Actor);
          break;
        }
      }
    }
  } else {
    CurrentSaveGame = Cast<USSaveGame>(
      UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass())
    );
    UE_LOG(LogTemp, Warning, TEXT("Created new save game data"));
  }

}

void ASGameModeBase::RespawnPlayerTimerElapsed(AController* Controller) {
  if (ensure(Controller)) {
    Controller->UnPossess();
    RestartPlayer(Controller);
  }
}

void ASGameModeBase::SpawnBotTimerElapsed() {
  if (!CVarSpawnBots.GetValueOnGameThread()) {
    UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
    return;
  }

  int32 NrOfAliveBots = 0;
  for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
    ASAICharacter* Bot = *It;

    USAttributeComponent* AttribComp = USAttributeComponent::GetAttributes(Bot);
    if (ensure(AttribComp) && AttribComp->IsAlive()) {
      NrOfAliveBots++;
    }
  }

  UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);

  float MaxBotCount = 10.f;
  if (ensure(DifficultyCurve)) {
    MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
  }

  if (NrOfAliveBots >= MaxBotCount) {
    UE_LOG(LogTemp, Log, TEXT("At max capacity, skipping bot spawn"), NrOfAliveBots);
    return;
  }

  UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
    this,
    SpawnBotQuery,
    this,
    EEnvQueryRunMode::RandomBest5Pct,
    nullptr
  );
  if (ensure(QueryInstance)) {
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
      this,
      &ASGameModeBase::OnBotSpawnQueryCompleted
    );
  }
}

void ASGameModeBase::OnBotSpawnQueryCompleted(
  UEnvQueryInstanceBlueprintWrapper* QueryInstance,
  EEnvQueryStatus::Type QueryStatus
) {
  if (QueryStatus != EEnvQueryStatus::Success) {
    UE_LOG(LogTemp, Warning, TEXT("SpawnBotEQSQuery Failed"));
    return;
  }

  TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

  if (Locations.IsValidIndex(0)) {
    if (MonsterTable) {
      TArray<FMonsterInfoRow*> Rows;
      MonsterTable->GetAllRows("", Rows);

      // Get random enemy
      int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
      FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

      DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.f);

      UAssetManager* Manager = UAssetManager::GetIfValid();
      if (Manager) {
        LogOnScreen(this, "Loading monster...", FColor::Green);
        const TArray<FName> Bundles;
        FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
          this,
          &ASGameModeBase::OnMonsterLoaded,
          SelectedRow->MonsterId,
          Locations[0]
        );

        Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
      }
    }
  }
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation) {
  UAssetManager* Manager = UAssetManager::GetIfValid();
  LogOnScreen(this, "Finished loading", FColor::Green);

  if (Manager) {
    USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
    if (MonsterData) {
      AActor* NewBot = GetWorld()->SpawnActor<AActor>(
        MonsterData->MonsterClass,
        SpawnLocation,
        FRotator::ZeroRotator
      );
      if (NewBot) {
        LogOnScreen(
          this,
          FString::Printf(
            TEXT("Spawned enemy: %s (%s)"),
            *GetNameSafe(NewBot),
            *GetNameSafe(MonsterData->MonsterClass)
          )
        );

        // Grant special actions, buffs etc.
        USActionComponent* ActionComp = Cast<USActionComponent>(
          NewBot->GetComponentByClass(USActionComponent::StaticClass())
        );
        if (ActionComp) {
          for (const TSubclassOf<USAction> ActionClass : MonsterData->Actions) {
            ActionComp->AddAction(NewBot, ActionClass);
          }
        }
      }
    }
  }

}

void ASGameModeBase::OnPowerUpSpawnQueryCompleted(
  UEnvQueryInstanceBlueprintWrapper* QueryInstance,
  EEnvQueryStatus::Type QueryStatus
) {
  if (QueryStatus != EEnvQueryStatus::Success) {
    UE_LOG(LogTemp, Warning, TEXT("Spawn PowerUp Query Failed"));
    return;
  }

  TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
  TArray<FVector> UsedLocations;
  int32 SpawnCounter = 0;

  while (SpawnCounter < DesiredPowerUpSpawnCount && Locations.Num() > 0) {
    const int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
    FVector SpawnLocation = Locations[RandomLocationIndex];
    Locations.RemoveAt(RandomLocationIndex);

    bool bValidLocation = true;
    for (FVector OtherLocation : UsedLocations) {
      const float DistanceTo = (SpawnLocation - OtherLocation).Size();
      if (DistanceTo < MinimumSpawnDistanceFromOrigin) {
        // Too close to already spawned PowerUp
        bValidLocation = false;
        break;
      }
    }

    // Failed distance check, go to next random location
    if (!bValidLocation) {
      continue;
    }

    const int32 RandomPowerUpClassIndex = FMath::RandRange(0, PowerUpClasses.Num() - 1);
    TSubclassOf<AActor> RandomPowerUpClass = PowerUpClasses[RandomPowerUpClassIndex];
    GetWorld()->SpawnActor<AActor>(RandomPowerUpClass, SpawnLocation, FRotator::ZeroRotator);

    UsedLocations.Add(SpawnLocation);
    SpawnCounter++;
  }
}
