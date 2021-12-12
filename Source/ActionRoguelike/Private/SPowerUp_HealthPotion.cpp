#include "SPowerUp_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion() {
  HealthAmount = 20.f;
  CreditsCost = 1;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
  Super::Interact_Implementation(InstigatorPawn);

  if (ensure(InstigatorPawn)) {
    USAttributeComponent* AttribComp = Cast<USAttributeComponent>(
      InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())
    );

    if (AttribComp->ApplyHealthChange(this, HealthAmount)) {
      ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
      if (ensure(PS)) {
        if (GetOwner()->HasAuthority()) {
          UE_LOG(LogTemp, Warning, TEXT("HAS ATHORITY REMOVING CREDIT"))
          PS->RemoveCredits(CreditsCost);
          bIsActive = false;
        }
        Deactivate();
      }
    }
  }
}
