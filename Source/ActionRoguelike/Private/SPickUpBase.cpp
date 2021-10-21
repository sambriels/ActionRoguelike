#include "SPickUpBase.h"

ASPickUpBase::ASPickUpBase() {
  SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
  SphereComp->SetSphereRadius(32.f);
  RootComponent = SphereComp;

  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  MeshComp->SetupAttachment(SphereComp);
}

void ASPickUpBase::Interact_Implementation(APawn* InstigatorPawn) {
  ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}
