#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
  TEXT("su.InteractionDebugDraw"),
  false,
  TEXT("Enable Debug lines for Interact component"),
  ECVF_Cheat
);

USInteractionComponent::USInteractionComponent() {
  PrimaryComponentTick.bCanEverTick = true;

  TraceRadius = 30.f;
  TraceDistance = 500.f;
  CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::PrimaryInteract() {
  ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus) {
  if (InFocus == nullptr) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, "No focussed actor to interact");
    return;
  }

  APawn* MyPawn = Cast<APawn>(GetOwner());
  ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

void USInteractionComponent::FindBestInteractable() {
  bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
  FCollisionObjectQueryParams ObjectQueryParams;
  ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

  AActor* MyOwner = GetOwner();

  FVector EyeLocation;
  FRotator EyeRotation;
  MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

  FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

  TArray<FHitResult> Hits;
  FCollisionShape Shape;
  float Radius = TraceRadius;
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

  // CLear ref before trying to fill
  FocusedActor = nullptr;

  for (FHitResult Hit : Hits) {
    AActor* HitActor = Hit.GetActor();

    if (HitActor) {
      if (HitActor->Implements<USGameplayInterface>()) {
        FocusedActor = HitActor;
        if (bDebugDraw) {
          DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 0.f);
        }
        break;
      }
    }
  }

  if (FocusedActor) {
    if (!DefaultWidgetInstance && ensure(DefaultWidgetClass)) {
      DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
    }

    if (DefaultWidgetInstance) {
      DefaultWidgetInstance->AttachedActor = FocusedActor;
      if (!DefaultWidgetInstance->IsInViewport()) {
        DefaultWidgetInstance->AddToViewport();
      }
    }
  } else {
    if (DefaultWidgetInstance) {
      DefaultWidgetInstance->RemoveFromParent();
    }
  }

  if (bDebugDraw) {
    DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
  }
}

void USInteractionComponent::TickComponent(
  float DeltaTime,
  ELevelTick Tick,
  FActorComponentTickFunction* ThisTickFunction
) {
  Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

  APawn* MyPawn = Cast<APawn>(GetOwner());
  if (MyPawn->IsLocallyControlled()) {
    FindBestInteractable();
  }

}
