#include "SCharacter.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ASCharacter::ASCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
  SpringArmComponent->bUsePawnControlRotation = true;
  SpringArmComponent->SetupAttachment(RootComponent);

  CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
  CameraComponent->SetupAttachment(SpringArmComponent);

  InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

  ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));

  GetCharacterMovement()->bOrientRotationToMovement = true;

  bUseControllerRotationYaw = false;

  HitFlashTimeParamName = "HitFlashTime";
  HitFlashColorParamName = "HitFlashColor";
}

void ASCharacter::PostInitializeComponents() {
  Super::PostInitializeComponents();
  AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

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

void ASCharacter::SprintStart() {
  ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop() {
  ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack() {
  ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::Dash() {
  ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::BlackHole() {
  ActionComp->StartActionByName(this, "BlackHole");
}

void ASCharacter::PrimaryInteract() {
  if (InteractionComp) {
    InteractionComp->PrimaryInteract();
  }
}

void ASCharacter::OnHealthChanged(
  AActor* InstigatorActor,
  USAttributeComponent* OwningComp,
  float NewHealth,
  float Delta
) {
  if (Delta < 0.f) {
    GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
    GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(1, 0, 0));

    if (NewHealth <= 0.f) {
      APlayerController* PC = Cast<APlayerController>(GetController());
      DisableInput(PC);

      SetLifeSpan(5.f);
    }
  }
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
  PlayerInputComponent->BindAction("BlackHole", IE_Pressed, this, &ASCharacter::BlackHole);
  PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction(
    "PrimaryInteract",
    IE_Pressed,
    this,
    &ASCharacter::PrimaryInteract
  );

  PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
  PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

FVector ASCharacter::GetPawnViewLocation() const {
  // return Super::GetPawnViewLocation();
  return CameraComponent->GetComponentLocation();
}

void ASCharacter::HealSelf(float Amount /* = 100.f */) {
  AttributeComp->ApplyHealthChange(this, Amount);
}
