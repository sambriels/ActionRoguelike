#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  void PrimaryInteract();
};
