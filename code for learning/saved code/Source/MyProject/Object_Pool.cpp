#include "Object_Pool.h"
#include "Engine/World.h"

AObjectPool::AObjectPool()
{
    // Set default values for properties if needed
}

void AObjectPool::BeginPlay()
{
    Super::BeginPlay();

    if (PooledObjectSubclass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("PooledObjectSubclass is not set in AObjectPool. Please set it in the Editor."));
        return;
    }

    UWorld* const World = GetWorld();
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned nullptr in AObjectPool::BeginPlay()."));
        return;
    }

    for (int i = 0; i < PoolSize; i++)
    {
        APooledObject* PoolableActor = World->SpawnActor<APooledObject>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (PoolableActor != nullptr)
        {
            PoolableActor->SetActive(false);
            PoolableActor->SetPoolIndex(i);
            PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &AObjectPool::OnPooledObjectDespawn);
            ObjectPool.Add(PoolableActor);
        }
    }
}

APooledObject* AObjectPool::SpawnPooledObject()
{
    for (APooledObject* PoolableActor : ObjectPool)
    {
        if (PoolableActor != nullptr && !PoolableActor->IsActive())
        {
            PoolableActor->TeleportTo(FVector::ZeroVector, FRotator::ZeroRotator);
            PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
            PoolableActor->SetActive(true);
            SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
            return PoolableActor;
        }
    }

    if (SpawnedPoolIndexes.Num() > 0)
    {
        int32 PooledObjectIndex = SpawnedPoolIndexes[0];
        SpawnedPoolIndexes.RemoveAt(0);
        APooledObject* PoolableActor = ObjectPool[PooledObjectIndex];
        if (PoolableActor != nullptr)
        {
            PoolableActor->SetActive(false);
            PoolableActor->TeleportTo(FVector::ZeroVector, FRotator::ZeroRotator);
            PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
            PoolableActor->SetActive(true);
            SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
            return PoolableActor;
        }
    }

    return nullptr;
}

void AObjectPool::OnPooledObjectDespawn(APooledObject* PoolActor)
{
    SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}
