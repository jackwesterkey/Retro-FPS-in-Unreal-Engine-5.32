#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.h" // Assuming APooledObject is defined here
#include "Object_Pool.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UObjectPoolComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UObjectPoolComponent();

    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    APooledObject* SpawnPooledObject();

    UFUNCTION()
    void OnPooledObjectDespawn(APooledObject* PoolActor);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Object Pool")
    TSubclassOf<APooledObject> PooledObjectSubclass;

    UPROPERTY(EditAnywhere, Category = "Object Pool")
    int32 PoolSize = 20;

    UPROPERTY(EditAnywhere, Category = "Object Pool")
    float PooledObjectLifeSpan = 0.0f;

    TArray<APooledObject*> ObjectPool;

    TArray<int32> SpawnedPoolIndexes;
};
