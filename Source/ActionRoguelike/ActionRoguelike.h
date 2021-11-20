// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(
  UObject* WorldContext,
  const FString Msg,
  const FColor Color = FColor::White,
  const float Duration = 5.f
) {
  if (!ensure(WorldContext)) {
    return;
  }

  UWorld* World = WorldContext->GetWorld();
  if (!ensure(World)) {
    return;
  }

  const FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client] " : "[Server] ";
  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
  }
}
