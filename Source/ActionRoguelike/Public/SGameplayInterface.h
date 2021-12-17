// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface {
  GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API ISGameplayInterface {
  GENERATED_BODY()

public:
  // Called after the Actor state was restored from SaveGame file
  UFUNCTION(BlueprintNativeEvent)
  void OnActorLoaded();

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
  FText GetInteractText(APawn* InstigatorPawn);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void Interact(APawn* InstigatorPawn);
};
