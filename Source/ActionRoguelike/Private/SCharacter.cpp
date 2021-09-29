// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
  SpringArmComponent->bUsePawnControlRotation = true;
  SpringArmComponent->SetupAttachment(RootComponent);

  CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
  CameraComponent->SetupAttachment(SpringArmComponent);

  InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

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
  FVector RightHandLocation = GetMesh()->GetSocketLocation("Muzzle_01");;
  FVector CameraLocation = CameraComponent->GetComponentLocation();

  /**
   * Since the location of the RightHand is different depending on the rotation of the character
   * (and the camera might be offset as well), we need to calculate the impact point of the attack
   * based on the camera location  and it's forward vector instead. This way it is much more intuitive
   * for the player to gage what he will hit
   */
  FHitResult Hit;
  FCollisionObjectQueryParams Params;
  Params.AddObjectTypesToQuery(ECC_WorldDynamic);
  Params.AddObjectTypesToQuery(ECC_WorldStatic);
  FVector TraceEnd = CameraLocation + CameraComponent->GetForwardVector() * 100000.f;
  bool bBLockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TraceEnd, Params);
  FVector HitLocation = bBLockingHit ? Hit.Location : TraceEnd;

  /** Calculates the rotation needed to arrive at the impact point from the trace */
  FRotator FinalRotation = FRotationMatrix::MakeFromX(HitLocation - RightHandLocation).Rotator();
  /**
   * This transformation matrix determines the final impact point, based on the start location
   * and the provided rotation
   */
  FTransform SpawnTM = FTransform(FinalRotation, RightHandLocation);
  FActorSpawnParameters SpawnParams;
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  SpawnParams.Instigator = this;
  GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
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
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

}
