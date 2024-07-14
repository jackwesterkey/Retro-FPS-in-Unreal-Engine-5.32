#include "Object_Pool.h"


UObjectPoolComponent::UObjectPoolComponent()
{
    // Constructor: Set default values for properties if needed
}

void UObjectPoolComponent::BeginPlay()
{
    Super::BeginPlay();

    // Check if PooledObjectSubclass is not set
    if (PooledObjectSubclass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("PooledObjectSubclass is not set in UObjectPoolComponent. Please set it in the Editor."));
        return;
    }

    UWorld* const World = GetWorld();
    // Check if GetWorld() returned nullptr
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr in UObjectPoolComponent::BeginPlay()."));
        return;
    }

    // Spawn objects for the pool
    for (int i = 0; i < PoolSize; i++)
    {
        APooledObject* PoolableActor = World->SpawnActor<APooledObject>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (PoolableActor != nullptr)
        {
            PoolableActor->SetActive(false);
            PoolableActor->SetPoolIndex(i);
            PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPoolComponent::OnPooledObjectDespawn);
            ObjectPool.Add(PoolableActor);
        }
    }
}

APooledObject* UObjectPoolComponent::SpawnPooledObject()
{
    // Iterate through the object pool to find an inactive object
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

    // If no inactive object is found, try to reuse a previously spawned one
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

    // If all else fails, return nullptr
    return nullptr;
}

void UObjectPoolComponent::OnPooledObjectDespawn(APooledObject* PoolActor)
{
    SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}
