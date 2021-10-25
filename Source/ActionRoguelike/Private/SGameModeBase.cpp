#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

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

void ASGameModeBase::SpawnBotTimerElapsed() {
  int32 NrOfAliveBots = 0;
  for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
    ASAICharacter* Bot = *It;

    USAttributeComponent* AttribComp = Cast<USAttributeComponent>(
      Bot->GetComponentByClass(USAttributeComponent::StaticClass())
    );
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
