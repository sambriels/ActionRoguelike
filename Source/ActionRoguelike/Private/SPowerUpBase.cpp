#include "SPowerUpBase.h"

ASPowerUpBase::ASPowerUpBase() {
  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  SphereComp->SetCollisionProfileName("PowerUp");
  RootComponent = SphereComp;

  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MeshComp->SetupAttachment(SphereComp);

  RespawnInterval = 5.f;
}

void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn) {
  ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}

void ASPowerUpBase::Deactivate() {
  RootComponent->ToggleVisibility(true);
  SetActorEnableCollision(false);

  FTimerHandle TimerHandle_Reactivate;
  GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &ASPowerUpBase::Reactivate, RespawnInterval);
}

void ASPowerUpBase::Reactivate() {
  RootComponent->ToggleVisibility(true);
  SetActorEnableCollision(true);
}
