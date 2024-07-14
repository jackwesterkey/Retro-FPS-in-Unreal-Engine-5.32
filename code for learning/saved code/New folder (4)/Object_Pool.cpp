// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" // Include necessary Unreal Engine header for AActor
#include "Object_Pool.h"

// Sets default values for this component's properties
UObject_Pool::UObject_Pool()
{
}

// Called when the game starts
void UObject_Pool::BeginPlay()
{
    Super::BeginPlay();

    // If PooledObjectSubclass is not set, return early
    if (PooledObjectSubclass == nullptr)
    {
        return;
    }

    UWorld* const World = GetWorld();

    // If World is not valid, return early
    if (World == nullptr)
    {
        return;
    }

    // Spawn objects and initialize the object pool
    for (int i = 0; i < PoolSize; i++)
    {
        APooledObject* PoolableActor = World->SpawnActor<APooledObject>(PooledObjectSubclass, FVector().ZeroVector, FRotator().ZeroRotator);

        if (PoolableActor != nullptr)
        {
            PoolableActor->SetActive(false);
            PoolableActor->SetPoolIndex(i);
            PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObject_Pool::OnPooledObjectDespawn);
            ObjectPool.Add(PoolableActor);
        }
    }
}

APooledObject* UObject_Pool::SpawnPooledObject()
{
    for (APooledObject* PoolableActor : ObjectPool)
    {
        if (PoolableActor != nullptr && !PoolableActor->IsActive())
        {
            PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
            PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
            PoolableActor->SetActive(true);
            SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());

            return PoolableActor;
        }
    }

    if (SpawnedPoolIndexes.Num() > 0)
    {
        int PooledObjectIndex = SpawnedPoolIndexes[0];
        SpawnedPoolIndexes.Remove(PooledObjectIndex);
        APooledObject* PoolableActor = ObjectPool[PooledObjectIndex];

        if (PoolableActor != nullptr)
        {
            PoolableActor->SetActive(false);
            PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
            PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
            PoolableActor->SetActive(true);
            SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());

            return PoolableActor;
        }
    }

    return nullptr;
}

void UObject_Pool::OnPooledObjectDespawn(APooledObject* PoolActor)
{
    SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}
