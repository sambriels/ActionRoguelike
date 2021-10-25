#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

void USInteractionComponent::PrimaryInteract() {
  FCollisionObjectQueryParams ObjectQueryParams;
  ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

  AActor* MyOwner = GetOwner();

  FVector EyeLocation;
  FRotator EyeRotation;
  MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

  FVector End = EyeLocation + (EyeRotation.Vector() * 1000.f);

  // FHitResult Hit;
  // bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

  TArray<FHitResult> Hits;
  FCollisionShape Shape;
  float Radius = 30.0f;
  Shape.SetSphere(Radius);

  bool bBlockingHit = GetWorld()->SweepMultiByObjectType(
    Hits,
    EyeLocation,
    End,
    FQuat::Identity,
    ObjectQueryParams,
    Shape
  );
  FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

  for (FHitResult Hit : Hits) {
    AActor* HitActor = Hit.GetActor();

    if (HitActor) {
      if (HitActor->Implements<USGameplayInterface>()) {
        APawn* MyPawn = Cast<APawn>(MyOwner);
        ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
        DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
        break;
      }
    }

  }

  // DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}
