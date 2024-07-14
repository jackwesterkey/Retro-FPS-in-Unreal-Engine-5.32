#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawn, APooledObject*, PoolActor);

UCLASS()
class MYPROJECT_API APooledObject : public AActor
{
    GENERATED_BODY()

public:
    APooledObject();

  
    FOnPooledObjectDespawn OnPooledObjectDespawn;

    UFUNCTION(BlueprintCallable, Category = "Pooled Object")
    void Deactivate();

    void SetActive(bool IsActive);
    void SetLifeSpan(float LifeTime);
    void SetPoolIndex(int Index);

    bool IsActive();
    int GetPoolIndex();

protected:
    bool Active;
    float LifeSpan;
    int PoolIndex;

    FTimerHandle LifeSpanTimer;
};
