#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


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

  virtual void TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
  ) override;

protected:
  UPROPERTY()
  TArray<USAction*> Actions;

  UPROPERTY(EditAnywhere, Category="Actions")
  TArray<TSubclassOf<USAction>> DefaultActions;

  virtual void BeginPlay() override;

};
