#include "SAction_ProjectileAttack.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack() {
  AttackAnimDelay = 0.2f;
  AttackSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator) {
  Super::StartAction_Implementation(Instigator);

  ACharacter* Character = Cast<ACharacter>(Instigator);

  if (ensure(Character)) {
    Character->PlayAnimMontage(AttackAnim);
    UGameplayStatics::SpawnEmitterAttached(
      CastingEffect,
      Character->GetMesh(),
      AttackSocketName,
      FVector::ZeroVector,
      FRotator::ZeroRotator,
      EAttachLocation::SnapToTarget
    );


    if (Character->HasAuthority()) {
      FTimerHandle TimerHandle_AttackDelay;
      FTimerDelegate Delegate;
      Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
      GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_AttackDelay,
        Delegate,
        AttackAnimDelay,
        false
      );
    }
  }

}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ASCharacter* InstigatorCharacter) {
  /**
  * Since the location of the RightHand is different depending on the rotation of the character
  * (and the camera might be offset as well), we need to calculate the impact point of the attack
  * based on the camera location  and it's forward vector instead. This way it is much more intuitive
  * for the player to gauge what he will hit
  */
  if (ensureAlways(ProjectileClass)) {
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = InstigatorCharacter;

    FCollisionShape Shape;
    Shape.SetSphere(20.f);

    // Ignore instigator
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(InstigatorCharacter);

    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjParams.AddObjectTypesToQuery(ECC_Pawn);

    FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
    FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000.f);

    FHitResult Hit;
    // Returns true if we found a blocking hit
    if (GetWorld()->SweepSingleByObjectType(
      Hit,
      TraceStart,
      TraceEnd,
      FQuat::Identity,
      ObjParams,
      Shape,
      Params
    )) {
      TraceEnd = Hit.ImpactPoint;
      DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Turquoise, false, 3.f);
    };

    FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(AttackSocketName);;
    // Calculates the rotation needed to arrive at the impact point from the trace
    FRotator FinalRotation = FRotationMatrix::MakeFromX(
      FVector(TraceEnd - HandLocation).GetSafeNormal()
    ).Rotator();

    // Determines the final impact point
    FTransform SpawnTransform = FTransform(FinalRotation, HandLocation);
    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
  }

  StopAction(InstigatorCharacter);
}
