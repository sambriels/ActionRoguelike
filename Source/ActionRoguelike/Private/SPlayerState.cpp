#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

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

int32 ASPlayerState::GetCredits() const {
  return Credits;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject) {
  if (SaveObject) {
    SaveObject->Credits = Credits;
  }
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject) {
  if (SaveObject) {
    AddCredits(SaveObject->Credits);
  }
}

void ASPlayerState::BeginPlay() {
  Super::BeginPlay();

  OnCreditsChanged.Broadcast(this, Credits, 0);
}

void ASPlayerState::OnRep_Credits(const int32 OldCredits) {
  OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME(ASPlayerState, Credits);
}
