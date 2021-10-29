#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
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
}

void ASGameModeBase::StartPlay() {
  Super::StartPlay();

  GetWorldTimerManager().SetTimer(
    TimerHandle_SpawnBots,
    this,
    &ASGameModeBase::SpawnBotTimerElapsed,
    SpawnTimerInterval,
    true
  );
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
  ASCharacter* Player = Cast<ASCharacter>(VictimActor);
  if (Player) {
    FTimerHandle TimerHandle_RespawnDelay;

    FTimerDelegate Delegate;
    Delegate.BindUFunction(this, "RespawnPlayerTimerElapsed", Player->GetController());

    float RespawnDelay = 2.f;
    GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
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
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
  }
}

void ASGameModeBase::OnQueryCompleted(
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
