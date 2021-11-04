#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
  TEXT("su.SpawnBots"),
  true,
  TEXT("Enable spawning of bots via timer."),
  ECVF_Cheat
);

ASGameModeBase::ASGameModeBase() {
  SpawnTimerInterval = 2.f;
  CreditsPerKill = 1;

  DesiredPowerUpSpawnCount = 20;
  MinimumSpawnDistanceFromOrigin = 200.f;

  PlayerStateClass = ASPlayerState::StaticClass();
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
      QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerUpSpawnQueryCompleted);
    }
  }

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

  UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
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
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
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
    DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 60.f);
    GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
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
