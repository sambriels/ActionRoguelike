#include "SItemChest.h"

#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest() {
  BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
  RootComponent = BaseMesh;

  LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
  LidMesh->SetupAttachment(BaseMesh);

  TargetPitch = 110.0f;

  bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) {
  bLidOpened = !bLidOpened;
  OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation() {
  ISGameplayInterface::OnActorLoaded_Implementation();
  OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened() {
  const float CurrentPitch = bLidOpened ? TargetPitch : 0.f;
  LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ASItemChest, bLidOpened);
}
