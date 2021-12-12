#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData {
  GENERATED_BODY()

  UPROPERTY()
  FString ActorName;

  UPROPERTY()
  FTransform Transform;

  UPROPERTY()
  TArray<uint8> BytesData;
};

UCLASS()
class ACTIONROGUELIKE_API USSaveGame : public USaveGame {
  GENERATED_BODY()

public:
  UPROPERTY()
  int32 Credits;

  UPROPERTY()
  TArray<FActorSaveData> SavedActors;
};
