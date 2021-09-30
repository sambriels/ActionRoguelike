#include "STargetDummy.h"

ASTargetDummy::ASTargetDummy() {
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
  RootComponent = MeshComp;

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
  AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(
  AActor* InstigatorActor,
  USAttributeComponent* OwningComp,
  float NewHealth,
  float Delta
) {
  if (Delta < 0) {
    MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
  }
}
