#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.h" // Assuming APooledObject is defined here
#include "Object_Pool.generated.h"

UCLASS()
class MYPROJECT_API AObjectPool : public AActor
{
    GENERATED_BODY()

public:
    AObjectPool();

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    APooledObject* SpawnPooledObject();

    UFUNCTION()
    void OnPooledObjectDespawn(APooledObject* PoolActor);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
    TSubclassOf<APooledObject> PooledObjectSubclass;

    UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
    int32 PoolSize = 20;

    UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
    float PooledObjectLifeSpan = 0.0f;

    TArray<APooledObject*> ObjectPool;

    TArray<int32> SpawnedPoolIndexes;
};
