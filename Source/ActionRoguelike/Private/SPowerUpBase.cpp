#include "SPowerUpBase.h"

#include "Net/UnrealNetwork.h"

ASPowerUpBase::ASPowerUpBase() {
  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  SphereComp->SetCollisionProfileName("PowerUp");
  RootComponent = SphereComp;

  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MeshComp->SetupAttachment(SphereComp);

  bReplicates = true;

  RespawnInterval = 5.f;
  bIsActive = true;
}

void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn) {
  ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}

FText ASPowerUpBase::GetInteractText_Implementation(APawn* InstigatorPawn) {
  return FText::GetEmpty();
}

void ASPowerUpBase::OnRep_IsActive() {
  RootComponent->SetVisibility(bIsActive, true);
  SetActorEnableCollision(bIsActive);
}

void ASPowerUpBase::Deactivate() {
  bIsActive = false;
  OnRep_IsActive();

  FTimerHandle TimerHandle_Reactivate;
  GetWorldTimerManager().SetTimer(
    TimerHandle_Reactivate,
    this,
    &ASPowerUpBase::Reactivate,
    RespawnInterval
  );
}

void ASPowerUpBase::Reactivate() {
  bIsActive = true;
  OnRep_IsActive();
}

void ASPowerUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME(ASPowerUpBase, bIsActive);
}
