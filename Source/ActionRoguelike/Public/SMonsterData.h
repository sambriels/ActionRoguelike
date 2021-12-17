#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SMonsterData.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API USMonsterData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn info")
  TSubclassOf<AActor> MonsterClass;

  /* Actions/buffs to grant this monster */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn info")
  TArray<TSubclassOf<USAction>> Actions;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
  UTexture2D* Icon;

  virtual FPrimaryAssetId GetPrimaryAssetId() const override {
    return FPrimaryAssetId("Monsters", GetFName());
  }
};
