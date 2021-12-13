#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USActionComponent;
class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
  FOnActionStateChanged,
  USActionComponent*,
  OwningComp,
  USAction*,
  Action
);

class USAction;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USActionComponent();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
  FGameplayTagContainer ActiveGameplayTags;

  UFUNCTION(BlueprintCallable, Category="Actions")
  void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

  UFUNCTION(BlueprintCallable, Category="Actions")
  USAction* GetAction(TSubclassOf<USAction> ActionToGet);

  UFUNCTION(BlueprintCallable, Category="Actions")
  void RemoveAction(USAction* ActionToRemove);

  UFUNCTION(BlueprintCallable, Category="Actions")
  bool StartActionByName(AActor* Instigator, FName ActionName);

  UFUNCTION(BlueprintCallable, Category="Actions")
  bool StopActionByName(AActor* Instigator, FName ActionName);

  UPROPERTY(BlueprintAssignable)
  FOnActionStateChanged OnActionStarted;

  UPROPERTY(BlueprintAssignable)
  FOnActionStateChanged OnActionStopped;

  virtual void TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
  ) override;

  virtual bool ReplicateSubobjects(
    UActorChannel* Channel,
    FOutBunch* Bunch,
    FReplicationFlags* RepFlags
  ) override;

protected:
  UFUNCTION(Server, Reliable)
  void ServerStartAction(AActor* Instigator, FName ActionName);

  UFUNCTION(Server, Reliable)
  void ServerStopAction(AActor* Instigator, FName ActionName);

  UPROPERTY(Replicated, BlueprintReadOnly)
  TArray<USAction*> Actions;

  UPROPERTY(EditAnywhere, Category="Actions")
  TArray<TSubclassOf<USAction>> DefaultActions;

  virtual void BeginPlay() override;

};
