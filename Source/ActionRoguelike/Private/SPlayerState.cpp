#include "SPlayerState.h"

ASPlayerState::ASPlayerState() {
  Credits = 5;
}

void ASPlayerState::AddCredits(const int32 Delta) {
  Credits += Delta;
  OnCreditsChanged.Broadcast(this, Credits, Delta);
}

void ASPlayerState::RemoveCredits(const int32 Delta) {
  Credits -= Delta;
  OnCreditsChanged.Broadcast(this, Credits, -Delta);
}

int32 ASPlayerState::GetCredits() {
  return Credits;
}

void ASPlayerState::BeginPlay() {
  Super::BeginPlay();

  OnCreditsChanged.Broadcast(this, Credits, 0);
}
