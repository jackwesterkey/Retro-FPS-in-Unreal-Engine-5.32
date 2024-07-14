// Fill out your copyright notice in the Description page of Project Settings.

#include "PooledObject.h"
#include "TimerManager.h" // Include TimerManager for managing timers

// Sets default values
APooledObject::APooledObject()
{
    // Set default values
    Active = false;
    LifeSpan = 0.0f;
    PoolIndex = -1; // Initialize PoolIndex to an invalid value
}

void APooledObject::SetActive(bool IsActive)
{
    Active = IsActive;
    SetActorHiddenInGame(!IsActive);

    if (IsActive)
    {
        // Start the lifespan timer if IsActive is true
        GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObject::Deactivate, LifeSpan, false);
    }
    else
    {
        // Clear the lifespan timer if IsActive is false
        GetWorldTimerManager().ClearTimer(LifeSpanTimer);
    }
}

void APooledObject::SetLifeSpan(float LifeTime)
{
    LifeSpan = LifeTime;
}

void APooledObject::SetPoolIndex(int Index)
{
    PoolIndex = Index;
}

void APooledObject::Deactivate()
{
    SetActive(false);
    OnPooledObjectDespawn.Broadcast(this);
}

bool APooledObject::IsActive() const
{
    return Active;
}

int APooledObject::GetPoolIndex() const
{
    return PoolIndex;
}
