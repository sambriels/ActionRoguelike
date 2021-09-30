#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASCharacter::ASCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
  SpringArmComponent->bUsePawnControlRotation = true;
  SpringArmComponent->SetupAttachment(RootComponent);

  CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
  CameraComponent->SetupAttachment(SpringArmComponent);

  InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

  GetCharacterMovement()->bOrientRotationToMovement = true;

  bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay() {
  Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;
  AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;

  const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

  AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack() {
  PlayAnimMontage(AttackAnim);
  GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed() {
  SpawnProjectile(PrimaryAttackProjectileClass);
}

void ASCharacter::DashProjectile() {
  PlayAnimMontage(AttackAnim);
  GetWorldTimerManager().SetTimer(TimerHandle_DashProjectile, this, &ASCharacter::DashProjectile_TimeElapsed, 0.2f);
}

void ASCharacter::DashProjectile_TimeElapsed() {
  SpawnProjectile(DashProjectileClass);
}

void ASCharacter::BlackHoleProjectile() {
  PlayAnimMontage(AttackAnim);
  GetWorldTimerManager().SetTimer(
    TimerHandle_BlackHoleProjectile,
    this,
    &ASCharacter::BlackHoleProjectile_TimeElapsed,
    0.2f
  );
}

void ASCharacter::BlackHoleProjectile_TimeElapsed() {
  SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass) {
  /**
  * Since the location of the RightHand is different depending on the rotation of the character
  * (and the camera might be offset as well), we need to calculate the impact point of the attack
  * based on the camera location  and it's forward vector instead. This way it is much more intuitive
  * for the player to gauge what he will hit
  */
  if (ensureAlways(ProjectileClass)) {
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = this;

    FCollisionShape Shape;
    Shape.SetSphere(0.1f);

    // Ignore player
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjParams.AddObjectTypesToQuery(ECC_Pawn);

    FVector TraceStart = CameraComponent->GetComponentLocation();
    FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.f);

    FHitResult Hit;
    // Returns true if we found a blocking hit
    if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params)) {
      TraceEnd = Hit.ImpactPoint;
      DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Turquoise, false, 3.f);
    };

    FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");;
    // Calculates the rotation needed to arrive at the impact point from the trace
    FRotator FinalRotation = FRotationMatrix::MakeFromX(FVector(TraceEnd - HandLocation).GetSafeNormal()).Rotator();

    // Determines the final impact point
    FTransform SpawnTransform = FTransform(FinalRotation, HandLocation);
    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
  }
}

void ASCharacter::PrimaryInteract() {
  if (InteractionComp) {
    InteractionComp->PrimaryInteract();
  }
}

// Called every frame
void ASCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  // -- Rotation Visualization -- //
  const float DrawScale = 100.0f;
  const float Thickness = 5.0f;

  FVector LineStart = GetActorLocation();
  // Offset to the right of pawn
  LineStart += GetActorRightVector() * 100.0f;
  // Set line end in direction of the actor's forward
  FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
  // Draw Actor's Direction
  DrawDebugDirectionalArrow(
    GetWorld(),
    LineStart,
    ActorDirection_LineEnd,
    DrawScale,
    FColor::Yellow,
    false,
    0.0f,
    0,
    Thickness
  );

  FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
  // Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
  DrawDebugDirectionalArrow(
    GetWorld(),
    LineStart,
    ControllerDirection_LineEnd,
    DrawScale,
    FColor::Green,
    false,
    0.0f,
    0,
    Thickness
  );

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
  PlayerInputComponent->BindAction("BlackHoleProjectile", IE_Pressed, this, &ASCharacter::BlackHoleProjectile);
  PlayerInputComponent->BindAction("DashProjectile", IE_Pressed, this, &ASCharacter::DashProjectile);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

}
