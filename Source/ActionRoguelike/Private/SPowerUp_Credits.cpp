#include "SPowerUp_Credits.h"

#include "SPlayerState.h"

ASPowerUp_Credits::ASPowerUp_Credits() {
  CreditsAmount = 1;
}

void ASPowerUp_Credits::Interact_Implementation(APawn* InstigatorPawn) {
  Super::Interact_Implementation(InstigatorPawn);
  ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
  if (ensure(PS)) {
    PS->AddCredits(CreditsAmount);
    Deactivate();
  }
}
