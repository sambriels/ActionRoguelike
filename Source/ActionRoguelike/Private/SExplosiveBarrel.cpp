#include "SExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "PhysicsEngine//RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel() {
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  MeshComp->SetSimulatePhysics(true);
  RootComponent = MeshComp;

  ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
  ForceComp->SetupAttachment(MeshComp);
  ForceComp->Radius = 750.f;
  ForceComp->ImpulseStrength = 2000.f;
  ForceComp->bImpulseVelChange = true;
  ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay() {
  Super::BeginPlay();
}

void ASExplosiveBarrel::PostInitializeComponents() {
  Super::PostInitializeComponents();
  MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(
  UPrimitiveComponent* HitComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  FVector NormalImpulse,
  const FHitResult& Hit
) {
  ForceComp->FireImpulse();

  UE_LOG(LogTemp, Log, TEXT("OnActorHit in ExplosiveBarrel"));

  UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

  FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
  DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}
