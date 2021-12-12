#include "SPlayerController.h"

void ASPlayerController::SetPawn(APawn* InPawn) {
  Super::SetPawn(InPawn);

  OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState() {
  Super::BeginPlayingState();

  BlueprintBeginPlayerState();
}
