#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;
UCLASS()
class ACTIONROGUELIKE_API USAnimInstance : public UAnimInstance {
  GENERATED_BODY()

protected:
  UPROPERTY(BlueprintReadOnly, Category="Animation")
  USActionComponent* ActionComp;

  /** Is Pawn stunned based on gameplay data */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
  bool bIsStunned;

  virtual void NativeInitializeAnimation() override;
  virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
