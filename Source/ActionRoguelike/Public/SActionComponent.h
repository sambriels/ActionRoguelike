#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USActionComponent();

  UFUNCTION(BlueprintCallable, Category="Actions")
  void AddAction(TSubclassOf<class USAction> ActionClass);

  UFUNCTION(BlueprintCallable, Category="Actions")
  bool StartActionByName(AActor* Instigator, FName ActionName);

  UFUNCTION(BlueprintCallable, Category="Actions")
  bool StopActionByName(AActor* Instigator, FName ActionName);

  virtual void TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
  ) override;

protected:
  UPROPERTY()
  TArray<class USAction*> Actions;

  UPROPERTY(EditAnywhere, Category="Actions")
  TArray<TSubclassOf<class USAction>> DefaultActions;

  virtual void BeginPlay() override;

};
