#include "SItemChest.h"

ASItemChest::ASItemChest() {
  BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
  RootComponent = BaseMesh;

  LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
  LidMesh->SetupAttachment(BaseMesh);

  TargetPitch = 110.0f;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) {
  LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}
